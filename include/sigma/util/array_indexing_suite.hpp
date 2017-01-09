#ifndef SIGMA_EXPORT_ARRAY_INDEXING_SUITE_HPP
#define SIGMA_EXPORT_ARRAY_INDEXING_SUITE_HPP

#include <boost/python/iterator.hpp>
#include <boost/python/list.hpp>
#include <boost/python/suite/indexing/container_utils.hpp>
#include <boost/python/suite/indexing/indexing_suite.hpp>

namespace sigma {
namespace python {

    template <class Container, bool NoProxy, class DerivedPolicies>
    class array_indexing_suite;

    namespace detail {
        template <class Container, bool NoProxy>
        class final_array_derived_policies : public array_indexing_suite<Container, NoProxy, final_array_derived_policies<Container, NoProxy> > {
        };
    }

    template <class Container, bool NoProxy = false, class DerivedPolicies = detail::final_array_derived_policies<Container, NoProxy> >
    class array_indexing_suite : public boost::python::indexing_suite<Container, DerivedPolicies, NoProxy> {
    public:
        typedef typename Container::value_type data_type;
        typedef typename Container::value_type key_type;
        typedef typename Container::size_type index_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::difference_type difference_type;

        template <class Class>
        static void extension_def(Class& cl)
        {
        }

        static typename boost::mpl::if_<boost::is_class<data_type>, data_type&, data_type>::type get_item(Container& container, index_type i)
        {
            return container[i];
        }

        static boost::python::object get_slice(Container& container, index_type from, index_type to)
        {
            if (from > to)
                return boost::python::list();

            boost::python::list list;
            while (from != to)
                list.append(container[from++]);
            return list;
        }

        static void set_item(Container& container, index_type i, data_type const& v)
        {
            container[i] = v;
        }

        static void set_slice(Container& container, index_type from, index_type to, data_type const& v)
        {
            if (from > to)
                return;

            while (from < to)
                container[from++] = v;
        }

        template <class Iter>
        static void set_slice(Container& container, index_type from, index_type to, Iter first, Iter last)
        {

            if (from > to)
                return;

            while (from < to)
                container[from++] = *first++;
        }

        static void delete_item(Container& container, index_type i)
        {
            set_item(container, i, data_type());
        }

        static void delete_slice(Container& container, index_type from, index_type to)
        {
            set_slice(container, from, to, data_type());
        }

        static size_t size(Container& container)
        {
            return container.size();
        }

        static bool contains(Container& container, key_type const& key)
        {
            return std::find(container.begin(), container.end(), key) != container.end();
        }

        static index_type get_min_index(Container&)
        {
            return 0;
        }

        static index_type get_max_index(Container& container)
        {
            return container.size();
        }

        static bool compare_index(Container&, index_type a, index_type b)
        {
            return a < b;
        }

        static index_type convert_index(Container& container, PyObject* i_)
        {
            boost::python::extract<long> i(i_);
            if (i.check()) {
                long index = i();
                if (index < 0)
                    index += DerivedPolicies::size(container);
                if (index >= long(container.size()) || index < 0) {
                    PyErr_SetString(PyExc_IndexError, "Index out of range");
                    boost::python::throw_error_already_set();
                }
                return index;
            }

            PyErr_SetString(PyExc_TypeError, "Invalid index type");
            boost::python::throw_error_already_set();
            return index_type();
        }
    };
}
}

#endif // SIGMA_EXPORT_ARRAY_INDEXING_SUITE_HPP
