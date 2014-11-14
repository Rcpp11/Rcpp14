#ifndef Rcpp__sugar__upper_tri_h
#define Rcpp__sugar__upper_tri_h

namespace Rcpp{
    namespace sugar{
    
        class UpperTri : 
            public SugarMatrixExpression<Rboolean,UpperTri>,
            custom_sugar_matrix_expression
            {
        public:
        
            UpperTri( int nr_, int nc_, bool diag) : nr(nr_), nc(nc_), keep_diag(diag){}
        
            inline Rboolean operator()( int i, int j ) const {
                return ( keep_diag ? (i>=j) : (i>j) ) ? TRUE : FALSE ;
            }
        
            inline R_xlen_t size() const { return nr * nc ; }
            inline int nrow() const { return nr; }
            inline int ncol() const { return nc; }
        
            template <typename Target>
            inline void apply( Target& target ) const {
                auto it = target.begin() ;
                if( keep_diag ){
                    for( int j=0; j<nc; j++){
                        for( int i=0; i<nr; i++, ++it ){
                            *it = (i>=j) ? TRUE : FALSE ;
                        }
                    }
                } else {
                    for( int j=0; j<nc; j++){
                        for( int i=0; i<nr; i++, ++it ){
                            *it = (i>j) ? TRUE : FALSE ;
                        }
                    }
                }
            }
        private:
            int nr, nc ;
            bool keep_diag ;
        
        } ;
    
    } // sugar
    
    template <typename eT, typename Expr>
    inline sugar::UpperTri
    upper_tri( const SugarMatrixExpression<eT, Expr>& lhs, bool diag = false){
        return sugar::UpperTri( lhs.nrow(), lhs.ncol(), diag ) ;
    }

} // Rcpp

#endif
