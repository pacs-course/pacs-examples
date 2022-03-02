/*
  Copyright (C) 2011,2016 Carlo de Falco
  This software is distributed under the terms
  the terms of the GNU/GPL licence v3
*/

#include "sparse_matrix.h"

void
sparse_matrix::extract_block_pointer (const std::vector<int> &rows,
                                      const std::vector<int> &cols,
                                      p_sparse_matrix &out)
{
  size_t  ii, jj;
  int jcol;
  std::map<int, double> *irow;
  out.resize (rows.size ());
  
  // copy the vector with the
  // list of columns into a map to
  // reduce the complexit of searches.
  // with this format the complexity
  // should be:
  // cols.size () * log (cols.size ())

  std::map<int, int> ordcol;
  
  for (jj = 0u; jj < cols.size (); ++jj)
    ordcol.insert (std::pair<int, int> (cols[jj], jj));

  for (ii = 0; ii < rows.size (); ++ii)
    // proceed only if the current row
    // is actually in the matrix
    if (rows[ii] < int ((*this).rows ()))
      {
        irow = &((*this)[rows[ii]]);
        if (irow->size ())
          // loop through nonzero entries of the current row
          // (constant complexity)
          for (auto jout = irow->begin (); jout != irow->end (); ++jout)
            {
              jcol = jout->first;
              // check if the current column is in the
              // list of selected columns (complexity: log (cols.size ()))
              if (ordcol.count (jcol))
                // insert a poiter to this entry in the output
                out[ii][ordcol.at (jcol)] = &((*irow)[jcol]);
            }
      }

  out.set_properties ();
}

void
sparse_matrix::extract_block_pointer_keep_cols
(const std::vector<int> &rows,
 const std::vector<int> &cols,
 p_sparse_matrix &out)
{
  size_t  ii, jj;
  int jcol;
  std::map<int, double> *irow;  
  out.resize (rows.size ());
  
  // same algorithm as for extract_block_pointer
  // except that we don't need to change the ordering
  // of columns, therefore we can use a std::set
  // instead of a std::map

  std::set<int> ordcol;
  
  for (jj = 0u; jj < cols.size (); ++jj)
    ordcol.insert (cols[jj]);

  for (ii = 0; ii < rows.size (); ++ii)
    if ((rows[ii] < (*this).rows ()))
      {
        irow = &((*this)[rows[ii]]);
        if (irow->size ())
          for (auto jout = irow->begin (); jout != irow->end (); ++jout)
            {
              jcol = jout->first;
              if (ordcol.count (jcol))
                out[ii][jcol] = &((*irow)[jcol]);
            }
      }

  out.set_properties ();
}

void sparse_matrix::reset ()
{
  double_sparse_matrix::row_iterator ii;
  double_sparse_matrix::col_iterator jj;
  for (ii = this->begin (); ii != this->end (); ++ii)
    for (jj = (*ii).begin (); jj != (*ii).end (); ++jj)
      (*jj).second = 0.0;
}

std::vector<double>
operator * (sparse_matrix& M, const std::vector<double>& x)
{
  std::vector<double> y (M.rows (), 0.0);
  sparse_matrix::col_iterator j;
  for (unsigned int i = 0; i < M.size (); ++i)
    if (M[i].size ())
      for (j = M[i].begin (); j != M[i].end (); ++j)
        y[i] += M.col_val (j) * x[M.col_idx (j)];

  return y;
}
