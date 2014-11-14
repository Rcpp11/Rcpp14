#ifndef Rcpp__sugar_lazy_fuse_h
#define Rcpp__sugar_lazy_fuse_h

namespace Rcpp{

    namespace sugar {

        template <int RTYPE, typename... Args>
        class Fuse : public LazyVector<RTYPE, Fuse<RTYPE, Args...>>{
        public:
            typedef typename traits::r_vector_element_converter<RTYPE>::type converter_type ;
            typedef std::tuple<Args...> Tuple ;

            Fuse( Args&&... args ) :
                data(std::forward<Args>(args)...),
                n(get_size(data)){}

            inline R_xlen_t size() const {
                return n ;
            }

            template <typename Target>
            inline void apply( Target& target ) const {
                auto it = target.begin() ;
                inplace_apply<0, decltype(it)>( it, std::true_type() ) ;
            }

        private:

            template <int INDEX, typename Iterator>
            void inplace_apply( Iterator& it, std::false_type ) const {}

            template <int INDEX, typename Iterator>
            void inplace_apply( Iterator& it, std::true_type ) const {
                typedef typename std::tuple_element<INDEX, Tuple>::type T ;
                inplace_apply_one<INDEX, Iterator, T>( it,
                    typename Rcpp::traits::is_primitive<T>::type()
                    );

                inplace_apply<INDEX+1,Iterator>( it,
                    typename std::integral_constant<bool, (INDEX+1 < sizeof...(Args)) >::type()
                    ) ;
            }

            template <int INDEX, typename Iterator, typename T>
            void inplace_apply_one( Iterator& it, std::false_type ) const {
                auto current = std::get<INDEX>(data) ;
                it = std::copy( current.begin(), current.end(), it ) ; 
            }

            template <int INDEX, typename Iterator, typename T>
            void inplace_apply_one( Iterator& it, std::true_type ) const {
                *it = std::get<INDEX>(data) ;
                ++it ;
            }

            Tuple data ;
            R_xlen_t n ;
        } ;


        template <typename... Args>
        struct fuse_type {
            typedef Fuse<
                create_r_type<Args...>::type::value,
                Args...
            > type ;
        } ;

    }

    template <typename... Args>
    typename sugar::fuse_type<Args...>::type
    fuse( Args&&... args ){
        return typename sugar::fuse_type<Args...>::type( std::forward<Args>(args)... );
    }

}
#endif
