#ifndef Rcpp__vector__impl_CharacterVector_h
#define Rcpp__vector__impl_CharacterVector_h

namespace Rcpp{
        
    #define VEC Vector<STRSXP,Storage>
    
    template <typename Storage>
    class Vector<STRSXP,Storage> :
        public VectorOfRTYPE<STRSXP>,
        public SugarVectorExpression<String,VEC>
    {
    public:
        typedef  String value_type ;
        typedef  internal::string_proxy<Vector>  Proxy    ;
        typedef  internal::Proxy_Iterator<Proxy> iterator ;
        typedef  internal::Proxy_Iterator<Proxy> const_iterator ;
        
        #define RTYPE STRSXP
        #include <Rcpp/vector/impl/RCPP_VECTOR_API.h>
        #undef RTYPE
    
    public:
        Vector( R_xlen_t n, const char* s ) {
            reset(n) ;
            std::fill( begin(), end(), Rf_mkChar(s) ) ;
        }
        
        Vector( const char* st ){
            reset(1) ;
            *begin() = st ;
        }         
        Vector( const std::string& st ) {
            reset(1) ;
            *begin() = st ;
        }
        
        Vector( std::initializer_list<const char*> list ){
            reset(list.size()) ;
            std::copy( list.begin(), list.end(), begin() ) ;
        }
        
        #include <Rcpp/vector/impl/RCPP_VECTOR_PROXY_BASED_API.h>
        
    } ;
    
    
    #undef VEC
  
}
#endif
