#include "SparseBlock.hpp"

namespace FVCode3D
{

SpMat sparseBlock(SpMat _matrix, UInt _ibegin, UInt _jbegin, UInt _icount, UInt _jcount)
{
    assert( int(_ibegin + _icount) <= _matrix.rows() );
    assert( int(_jbegin + _jcount) <= _matrix.cols() );

    UInt Mj, Mi, i, currOuterIndex, nextOuterIndex;
    std::vector<Triplet> tripletList;
    tripletList.reserve(_matrix.nonZeros());

    for(UInt j = 0; j < _jcount; j++)
    {
        Mj = j + _jbegin;
        currOuterIndex = _matrix.outerIndexPtr()[Mj];
        nextOuterIndex = _matrix.outerIndexPtr()[Mj+1];

        for(UInt a = currOuterIndex; a < nextOuterIndex; a++)
        {
            Mi = _matrix.innerIndexPtr()[a];

            if(Mi < _ibegin)
            {
                continue;
            }
            if(Mi >= _ibegin + _icount)
            {
                break;
            }

            i = Mi - _ibegin;
            tripletList.emplace_back( i, j, _matrix.valuePtr()[a] ); // Triplet
        }
    }
    SpMat matS(_icount, _jcount);
    matS.setFromTriplets( std::begin( tripletList ), std::end( tripletList ) );
    return matS;
}

SpMat sparseTopLeftBlock(SpMat _matrix, UInt _icount, UInt _jcount)
{
    return sparseBlock( _matrix, 0, 0, _icount, _jcount );
}

SpMat sparseTopRightBlock(SpMat _matrix, UInt _icount, UInt _jcount)
{
    return sparseBlock( _matrix, 0, _matrix.cols()-_jcount, _icount, _jcount );
}

SpMat sparseBottomLeftBlock(SpMat _matrix, UInt _icount, UInt _jcount)
{
    return sparseBlock( _matrix, _matrix.rows() - _icount, 0, _icount, _jcount );
}

SpMat sparseBottomRightBlock(SpMat _matrix, UInt _icount, UInt _jcount)
{
    return sparseBlock( _matrix, _matrix.rows() - _icount, _matrix.cols() - _jcount, _icount, _jcount );
}

} // namespace FVCode3D
