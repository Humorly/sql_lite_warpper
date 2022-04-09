#pragma once

#include <tuple>
#include <vector>
#include "wstddef.h"

class sql_operate_ipml
{
public:
    sql_operate_ipml() {}
    virtual ~sql_operate_ipml() {}

protected:
    template <typename __dest_type, typename __value_type, typename T, typename ... params>
    std::vector<__dest_type>& separation(std::vector<__dest_type> & dest, __value_type& val)
    {
        __dest_type tu_;
        constexpr int size_ = sizeof...(params);
        get_class<0, size_>::template get<__dest_type, __value_type, T, params...>(tu_, val);

        dest.push_back(tu_);
        return dest;
    }

    // 展开参数
    template<integer64_type N, integer64_type M>
    struct get_class
    {
        template <typename __dest, typename __value_type, typename T, typename ... params>
        inline static void get(__dest& dest, __value_type& val)
        {
            constexpr integer64_type index_ = N + 1;
            // 赋值元素N并递归展开
            std::get<N>(dest) = get_value_type<T, __value_type, integer64_type>::get_value(val, N);
            get_class<index_, M>::template get<__dest, __value_type, params...>(dest, val);
        }
    };

    // 递归到末元素
    template<integer64_type M>
    struct get_class<M, M>
    {
        template <typename __dest, typename __value_type, typename T>
        inline static void get(__dest& dest, __value_type& val)
        {
            // 末元素赋值
            std::get<M>(dest) = get_value_type<T, __value_type, integer64_type>::get_value(val, M);
        }
    };

    // 分离型别偏特化
    template<typename __part_type, typename __value_type, typename __parm>
    struct get_value_type
    {
        inline static void get_value(__value_type& val, __parm parm) {}
    };

    // bool
    template<typename __value_type, typename __parm>
    struct get_value_type<bool, __value_type, __parm>
    {
        inline static bool get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toBool();
        }
    };

    // int
    template<typename __value_type, typename __parm>
    struct get_value_type<int, __value_type, __parm>
    {
        inline static int get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toInt();
        }
    };

    // unsigned int
    template<typename __value_type, typename __parm>
    struct get_value_type<unsigned int, __value_type, __parm>
    {
        inline static unsigned int get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toUInt();
        }
    };

    // unsigned int 64
    template<typename __value_type, typename __parm>
    struct get_value_type<unsigned long long, __value_type, __parm>
    {
        inline static unsigned long long get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toULongLong();
        }
    };

    // bigint
    template<typename __value_type, typename __parm>
    struct get_value_type<integer64_type, __value_type, __parm>
    {
        inline static integer64_type get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toLongLong();
        }
    };

    // varchar
    template<typename __value_type, typename __parm>
    struct get_value_type<std::string, __value_type, __parm>
    {
        inline static std::string get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toString().toStdString();
        }
    };

    // double
    template<typename __value_type, typename __parm>
    struct get_value_type<double, __value_type, __parm>
    {
        inline static double get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toDouble();
        }
    };

    // QString
    template<typename __value_type, typename __parm>
    struct get_value_type<QString, __value_type, __parm>
    {
        inline static QString get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toString();
        }
    };

    // QDate
    template<typename __value_type, typename __parm>
    struct get_value_type<QDate, __value_type, __parm>
    {
        inline static QString get_value(__value_type& val, __parm parm)
        {
            return val.value(parm).toDate();
        }
    };

    // 合成参数
    template <typename __value_type, typename ... params>
    void synthesis(__value_type* statement, std::tuple<params...>& node)
    {
        constexpr integer64_type N = sizeof...(params);
        make_class<0, N - 1>::template make<__value_type, std::tuple<params...>, params...>(statement, node);
    }

    template <typename __value_type>
    void synthesis(__value_type* statement, std::tuple<>& node)
    {
    }

    // 展开参数
    template<integer64_type N, integer64_type M>
    struct make_class
    {
        template <typename __value_type, typename __set, typename __part_type, typename ... params>
        inline static void make(__value_type* statement, __set& node)
        {
            // 赋值元素N并递归展开
            constexpr integer64_type index_ = N + 1;
            make_value_type<__part_type, __value_type, __part_type>::set_value(statement, std::get<N>(node));
            make_class<index_, M>::template make<__value_type, __set, params...>(statement, node);
        }
    };

    // 递归到末元素
    template<integer64_type M>
    struct make_class<M, M>
    {
        template <typename __value_type, typename __set, typename __part_type, typename ... params>
        inline static void make(__value_type* statement, __set& node)
        {
            // 末元素赋值
            make_value_type<__part_type, __value_type, __part_type>::set_value(statement, std::get<M>(node));
        }
    };

    // 合成型别偏特化
    template<typename __part_type, typename __value_type, typename __parm>
    struct make_value_type
    {
        inline static void set_value(__value_type* statement, __parm parm)
        {
            statement->addBindValue(parm);
        }
    };

    // std::string
    template<typename __value_type, typename __parm>
    struct make_value_type<std::string, __value_type, __parm>
    {
        inline static void set_value(__value_type* statement, __parm parm)
        {
            statement->addBindValue(parm.c_str());
        }
    };

    // 展开tuple
    template <integer64_type N, typename T, typename ... params>
    inline static std::vector<std::string> expand(std::tuple<T, params...>& parm)
    {
        std::vector<std::string> dest_;
        constexpr int size_ = sizeof...(params);
        // 展开tuple
        get_tuple<N, size_>::template get<std::tuple<T, params...>>(dest_, parm);

        return dest_;
    }

    // 展开参数
    template<integer64_type N, integer64_type M>
    struct get_tuple
    {
        template <typename __set>
        inline static void get(std::vector<std::string>& dest, __set& parm)
        {
            constexpr integer64_type index_ = N + 1;
            // 赋值元素N并递归展开
            dest.push_back(std::get<N>(parm));
            get_tuple<index_, M>::template get<__set>(dest, parm);
        }
    };

    // 递归到末元素
    template<integer64_type M>
    struct get_tuple<M, M>
    {
        template <typename __set>
        inline static void get(std::vector<std::string>& dest, __set& parm)
        {
            // 末元素赋值
            dest.push_back(std::get<M>(parm));
        }
    };
};

