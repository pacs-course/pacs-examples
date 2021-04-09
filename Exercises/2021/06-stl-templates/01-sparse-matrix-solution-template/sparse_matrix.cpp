#include "sparse_matrix.hpp"

void
sparse_matrix::extract_block_pointer(const std::vector<unsigned int> &rows,
                                     const std::vector<unsigned int> &cols,
                                     p_sparse_matrix &                out)
{
  size_t                          ii, jj;
  unsigned int                    jcol;
  std::map<unsigned int, double> *irow;
  out.resize(rows.size());

  // copy the vector with the list of columns into a map to
  // reduce the complexity of searches.
  // with this format the complexity should be:
  // cols.size() * log(cols.size())

  std::map<unsigned int, unsigned int> ordcol;

  for (jj = 0; jj < cols.size(); ++jj)
    ordcol.insert(std::pair<unsigned int, unsigned int>(cols[jj], jj));

  for (ii = 0; ii < rows.size(); ++ii)
    // proceed only if the current row is actually in the matrix
    if (rows[ii] < static_cast<unsigned int>((*this).rows()))
      {
        irow = &((*this)[rows[ii]]);
        // loop through nonzero entries of the current row
        // (constant complexity)
        for (auto jout = irow->begin(); jout != irow->end(); ++jout)
          {
            jcol = jout->first;
            // check if the current column is in the
            // list of selected columns
            // (complexity: log(cols.size()))
            if (ordcol.count(jcol))
              // insert a pointer to this entry in the output
              out[ii][ordcol.at(jcol)] = &((*irow)[jcol]);
          }
      }

  out.update_properties();
}

void
sparse_matrix::extract_block_pointer_keep_cols(
  const std::vector<unsigned int> &rows,
  const std::vector<unsigned int> &cols,
  p_sparse_matrix &                out)
{
  size_t                          ii, jj;
  unsigned int                    jcol;
  std::map<unsigned int, double> *irow;
  out.resize(rows.size());

  // same algorithm as for extract_block_pointer except that we don't need to
  // change the ordering of columns, therefore we can use a std::set instead of
  // a std::map

  std::set<unsigned int> ordcol;

  for (jj = 0; jj < cols.size(); ++jj)
    ordcol.insert(cols[jj]);

  for (ii = 0; ii < rows.size(); ++ii)
    if ((static_cast<size_t>(rows[ii]) < (*this).rows()))
      {
        irow = &((*this)[rows[ii]]);
        for (auto jout = irow->begin(); jout != irow->end(); ++jout)
          {
            jcol = jout->first;
            if (ordcol.count(jcol))
              out[ii][jcol] = &((*irow)[jcol]);
          }
      }

  out.update_properties();
}

void
sparse_matrix::reset()
{
  for (auto ii : *this)
    for (auto &jj : ii)
      jj.second = 0.0;
}

std::vector<double>
operator*(sparse_matrix &M, const std::vector<double> &x)
{
  std::vector<double> y(M.rows(), 0.0);

  for (unsigned int i = 0; i < M.size(); ++i)
    for (auto j = M[i].begin(); j != M[i].end(); ++j)
      y[i] += M.col_val(j) * x[M.col_idx(j)];

  return y;
}
