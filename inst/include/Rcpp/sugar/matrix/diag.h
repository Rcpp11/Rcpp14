#ifndef Rcpp__sugar__diag_h
#define Rcpp__sugar__diag_h

namespace Rcpp{
    namespace sugar{
    
        template <typename eT, typename Expr>
        class Diag_Extractor : public SugarVectorExpression<eT, Diag_Extractor<eT, Expr>> {
        public:
            typedef indexing_iterator<eT, Diag_Extractor> const_iterator ;
            
            Diag_Extractor( const SugarMatrixExpression<eT, Expr>& object_ ) : object(object_), n(0) {
                int nr = object.nrow() ;
                int nc = object.ncol() ;
                n = (nc < nr ) ? nc : nr ;
            }
        
            inline eT operator[]( R_xlen_t i ) const {
                return object( i, i ) ;
            }
            inline R_xlen_t size() const { return n; }
        
            inline const_iterator begin() const { return const_iterator( *this, 0 ) ; }
            inline const_iterator end() const { return const_iterator( *this, size() ) ; }
            
        private:
            const SugarMatrixExpression<eT, Expr>& object ;
            int n ;
        } ;
        
        
        template <typename eT, typename Expr>
        class Diag_Maker : 
            public Rcpp::SugarMatrixExpression< eT, Diag_Maker<eT, Expr> >, 
            public custom_sugar_matrix_expression {
        public:
            Diag_Maker( const SugarVectorExpression<eT, Expr>& object_ ) : 
                object(object_), start(object.get_ref().begin()), n(object_.size()) {}
        
            inline eT operator()( int i, int j ) const {
                return (i==j) ? start[i] : 0 ;
            }
            inline R_xlen_t size() const { return n * n; }
            inline int ncol() const { return n; }
            inline int nrow() const { return n; }
               
            template <typename Target>
            inline void apply( Target& target ) const {
                auto it = target.begin() ; 
                auto source = sugar_begin(object) ;
                for( int j=0; j<n-1; j++, ++source ){
                    *it = *source ; 
                    it = std::fill_n(it + 1, n, 0 ); 
                }
                *it = *source ;
            }
            
        private:
            const SugarVectorExpression<eT, Expr>& object ;
            typename Expr::const_iterator start ;
            int n ;
        } ;
    
    } // sugar
    
    
    template <typename eT, typename Expr>
    inline sugar::Diag_Maker<eT, Expr>
    diag( const SugarVectorExpression<eT, Expr>& x ){
        return sugar::Diag_Maker<eT, Expr>(x) ;    
    }
    
    template <typename eT, typename Expr>
    inline sugar::Diag_Extractor<eT, Expr>
    diag( const SugarMatrixExpression<eT ,Expr>& mat){
        return sugar::Diag_Extractor<eT, Expr>(mat) ;    
    }

} // Rcpp
#endif

