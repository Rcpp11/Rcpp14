#ifndef Rcpp__internal__exporter__h
#define Rcpp__internal__exporter__h

namespace Rcpp{
    namespace traits{

        template <typename T> class Exporter{
        public:
            Exporter( SEXP x ) : t(x){}
            inline T get(){ return t ; }
    
        private:
            T t ;
        } ;
    
        template <typename T> class RangeExporter {
        public:
            typedef typename T::value_type r_export_type ;
    
            RangeExporter( SEXP x ) : object(x){}
            ~RangeExporter(){}
    
            T get(){ 
                T vec( ::Rf_xlength(object) );
                ::Rcpp::internal::export_range( object, vec.begin() ) ;
                return vec ;
            }
    
        private:
            SEXP object ;
        } ;
        
        template <typename T, typename value_type> class IndexingExporter {
        public:
            typedef value_type r_export_type ;
        
            IndexingExporter( SEXP x) : object(x){}
            ~IndexingExporter(){}
        
            T get(){
                T result( ::Rf_xlength(object) ) ;
                ::Rcpp::internal::export_indexing<T,value_type>( object, result ) ;
                return result ;
            }
        
        private:
            SEXP object ;
        } ;

        template <typename T, typename value_type> class MatrixExporter {
        public:
            typedef value_type r_export_type ;
        
            MatrixExporter( SEXP x) : object(x){}
            ~MatrixExporter(){}
        
            T get() {
                SEXP dims = ::Rf_getAttrib( object, R_DimSymbol ) ;
                if( dims == R_NilValue || ::Rf_xlength(dims) != 2 ){
                    stop("not a matrix") ;
                }
                int* dims_ = INTEGER(dims) ;
                T result( dims_[0], dims_[1] ) ;
                ::Rcpp::internal::export_indexing<T,value_type>( object, result ) ;
                return result ;
            }
        
        private:
            SEXP object ;
        } ;

        template < template<class,class> class Container, typename T>
        struct container_exporter{
                typedef RangeExporter< Container<T, std::allocator<T> > > type ;
        } ;
        
        template < template <class, class> class ContainerTemplate, typename T > class ContainerExporter {
        public:
            typedef ContainerTemplate<T, std::allocator<T> > Container ;
            const static int RTYPE = Rcpp::traits::r_sexptype_traits<T>::rtype ;
            
            ContainerExporter( SEXP x ) : object(x){}
            ~ContainerExporter(){}
            
            Container get(){
                if( TYPEOF(object) == RTYPE ){
                   T* start = Rcpp::internal::r_vector_start<RTYPE>(object) ;
                   return Container( start, start + Rf_xlength(object) ) ;
                }
                Container vec( ::Rf_xlength(object) );
                ::Rcpp::internal::export_range( object, vec.begin() ) ;
                return vec ;
            }
        
        private:
                SEXP object ;
        } ;
        template < template<class,class> class Container > struct container_exporter< Container, int >{
            typedef ContainerExporter< Container, int > type ;     
        } ;
        template < template<class,class> class Container > struct container_exporter< Container, double >{
            typedef ContainerExporter< Container, double > type ;
        } ;
        
        template <typename T> class Exporter< std::vector<T> > : public container_exporter< std::vector, T>::type {
        public:
            Exporter(SEXP x) : container_exporter< std::vector, T>::type(x){}
        };
        template <typename T> class Exporter< std::deque<T> > : public container_exporter< std::deque, T>::type {
        public:
            Exporter(SEXP x) : container_exporter< std::deque, T>::type(x){}
        };
        template <typename T> class Exporter< std::list<T> > : public container_exporter< std::list, T>::type {
        public:
            Exporter(SEXP x) : container_exporter< std::list, T>::type(x){}
        };
        
    } // namespace traits
} // namespace Rcpp
#endif
