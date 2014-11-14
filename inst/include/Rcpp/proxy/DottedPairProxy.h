#ifndef Rcpp_DottedPairProxy_h
#define Rcpp_DottedPairProxy_h

namespace Rcpp {
    
    template <typename CLASS>
    class DottedPairProxyPolicy {
    public:
        
        class DottedPairProxy : public GenericProxy<DottedPairProxy> {
        public:
            DottedPairProxy( CLASS& v, R_xlen_t index_ ) : node(R_NilValue){
                if( index_ >= v.length() ) stop("index out of bounds") ;
                SEXP x = v ; /* implicit conversion */
                for( R_xlen_t i = 0; i<index_; i++, x = CDR(x) ) ;
                node = x ;
            }
            
            DottedPairProxy& operator=(const DottedPairProxy& rhs){
                return set(rhs.get());    
            }
            DottedPairProxy& operator=(SEXP rhs){
                return set(rhs) ;    
            }
            
            template <typename T>
            DottedPairProxy& operator=(const T& rhs) {
                return set(wrap(rhs)) ;
            }
            
            template <typename T>
            DottedPairProxy& operator=(const traits::named_object<T>& rhs) {
                return set(wrap(rhs.object), rhs.name) ;
            }
            
            template <typename T> operator T() const {
                return as<T>(get());  
            }
            
            inline SEXP get() const { 
                return CAR(node); 
            }
            inline operator SEXP() const { 
                return get() ; 
            }
            inline DottedPairProxy& set(SEXP x){ 
                SETCAR( node, x ) ; 
                return *this ;
            } 
            inline DottedPairProxy& set(SEXP x, SEXP name){
                SETCAR( node, x ) ;
                SEXP rhsNameSym = name ;
                SET_TAG( node, rhsNameSym ) ;
                return *this ;
            }
            
        private:
            SEXP node ;
        } ;
    
        DottedPairProxy operator[]( R_xlen_t i){
            return DottedPairProxy( static_cast<CLASS&>(*this), i ) ;    
        }
        const DottedPairProxy operator[](R_xlen_t i) const{
            return DottedPairProxy( const_cast<CLASS&>(static_cast<const CLASS&>(*this)), i ) ;
        }
        
    } ;

}
#endif
