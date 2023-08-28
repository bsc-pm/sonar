/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include "Manager.hpp"
#include "Symbol.hpp"

#define DEFINE_FUNC0(lang, op, cnt, ret, name)                                 \
    ret name(void)                                                             \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(#name);                    \
    }

#define DEFINE_FUNC1(lang, op, cnt, ret, name, t0)                             \
    ret name(t0 p0)                                                            \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(#name, p0);                \
    }

#define DEFINE_FUNC2(lang, op, cnt, ret, name, t0, t1)                         \
    ret name(t0 p0, t1 p1)                                                     \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(#name, p0, p1);            \
    }

#define DEFINE_FUNC3(lang, op, cnt, ret, name, t0, t1, t2)                     \
    ret name(t0 p0, t1 p1, t2 p2)                                              \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(#name, p0, p1, p2);        \
    }

#define DEFINE_FUNC4(lang, op, cnt, ret, name, t0, t1, t2, t3)                 \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3)                                       \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(#name, p0, p1, p2, p3);    \
    }

#define DEFINE_FUNC5(lang, op, cnt, ret, name, t0, t1, t2, t3, t4)             \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4)                                \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4);                                    \
    }

#define DEFINE_FUNC6(lang, op, cnt, ret, name, t0, t1, t2, t3, t4, t5)         \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4, t5 p5)                         \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4, p5);                                \
    }

#define DEFINE_FUNC7(lang, op, cnt, ret, name, t0, t1, t2, t3, t4, t5, t6)     \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6)                  \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4, p5, p6);                            \
    }

#define DEFINE_FUNC8(lang, op, cnt, ret, name, t0, t1, t2, t3, t4, t5, t6, t7) \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7)           \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4, p5, p6, p7);                        \
    }

#define DEFINE_FUNC9(lang, op, cnt, ret, name, t0, t1, t2, t3, t4, t5, t6, t7, \
                     t8)                                                       \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8)    \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4, p5, p6, p7, p8);                    \
    }

#define DEFINE_FUNC10(lang, op, cnt, ret, name, t0, t1, t2, t3, t4, t5, t6,    \
                      t7, t8, t9)                                              \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8,    \
             t9 p9)                                                            \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);                \
    }

#define DEFINE_FUNC11(lang, op, cnt, ret, name, t0, t1, t2, t3, t4, t5, t6,    \
                      t7, t8, t9, t10)                                         \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8,    \
             t9 p9, t10 p10)                                                   \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);           \
    }

#define DEFINE_FUNC12(lang, op, cnt, ret, name, t0, t1, t2, t3, t4, t5, t6,    \
                      t7, t8, t9, t10, t11)                                    \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8,    \
             t9 p9, t10 p10, t11 p11)                                          \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);      \
    }

#define DEFINE_FUNC13(lang, op, cnt, ret, name, t0, t1, t2, t3, t4, t5, t6,    \
                      t7, t8, t9, t10, t11, t12)                               \
    ret name(t0 p0, t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, t7 p7, t8 p8,    \
             t9 p9, t10 p10, t11 p11, t12 p12)                                 \
    {                                                                          \
        return Manager::process<lang, op, cnt, ret>(                           \
                #name, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); \
    }

#endif // DEFINITIONS_HPP
