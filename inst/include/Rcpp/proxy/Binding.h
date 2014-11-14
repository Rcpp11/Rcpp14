#ifndef Rcpp_proxy_EnvironmentBinding_h
#define Rcpp_proxy_EnvironmentBinding_h

namespace Rcpp{
    
    template <typename EnvironmentClass>
    class Binding : public GenericProxy<Binding<EnvironmentClass>> {
    public:
        Binding( EnvironmentClass& env_, std::string  name_) : 
            env(env_), name(std::move(name_)){}
        
        inline bool active() const { 
            return env.bindingIsActive(name) ;    
        }
        inline bool locked() const {
            return env.bindingIsLocked(name) ;    
        }
        inline bool exists() const {
            return env.exists(name) ;    
        }
        void lock() {
            env.lockBinding(name) ;    
        }
        void unlock(){
            env.unlockBinding(name) ;    
        }
        Binding& operator=(const Binding& rhs){
            if( *this != rhs )
                set( rhs.get() ) ;
            return *this ;
        }
        
        template <typename T> 
        Binding& operator=(const T& rhs) {
            set( wrap(rhs) ) ;
            return *this ;
        }
        
        template <typename T> operator T() const{
            return as<T>( get() ) ;  
        }
        
    private:
        
        SEXP get() const {
            return env.get( name ) ;    
        }
        
        void set( SEXP x){
            env.assign(name, x ) ;     
        }
        
        EnvironmentClass& env ;
        std::string name ;
    } ;
    
}
#endif
