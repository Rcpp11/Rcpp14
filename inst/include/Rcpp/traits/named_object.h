#ifndef Rcpp__traits__named_object__h
#define Rcpp__traits__named_object__h

namespace Rcpp{
    class Argument ;
    
    namespace traits{
    
        template <typename T> 
        class named_object {
        public:
            typedef T object_type ;
            named_object( SEXP name_, const T& o_) : 
                name(name_), object(o_){}
            named_object( const std::string& name_, const T& o_) : 
                name(Rf_install(name_.c_str())), object(o_){}
            SEXP name ;
            const T& object ;
            
            template <typename U>
            inline operator named_object<U>() {
                return named_object<U>( CHAR(PRINTNAME(name)), static_cast<U>(object) ) ;   
            }
            
        } ;
        
        template <> 
        class named_object<SEXP> {
        public:
            typedef SEXP object_type ;
            
            named_object( SEXP name_, const SEXP& o_): name(name_), object(o_) {
               R_PreserveObject(object) ;
            }
            named_object( const std::string& name_, const SEXP& o_): name( Rf_install(name_.c_str()) ), object(o_){
               R_PreserveObject(object) ;
            }
            named_object( const named_object<SEXP>& other ) : name(other.name), object(other.object){
               R_PreserveObject(object) ;
            }
            ~named_object(){
               R_ReleaseObject(object) ;
            }
        
            SEXP name ;
            SEXP object ;
        } ;
        
        
        template <typename T> struct is_named : public std::false_type{} ;
        template <typename T> struct is_named< named_object<T> >   : public std::true_type {} ;
        template <> struct is_named< Rcpp::Argument >   : public std::true_type {} ;

    } // namespace traits

    template <typename T>
    struct Demangler< Rcpp::traits::named_object<T> > {
        static std::string get(){
            std::string res = "Rcpp::traits::named_object<" ;
            res += Demangler<T>::get() ;
            res += ">" ;
            return res ;
        }
    } ;
    

} // namespace Rcpp

#endif 
