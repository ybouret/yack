include_directories(../../lua++ ../..)

file(GLOB global global/*.cpp)
source_group(global FILES ${global} )

file(GLOB algebra algebra/*.cpp)
source_group(algebra FILES ${algebra} )

file(GLOB root root/*.cpp)
source_group(root FILES ${root} )

file(GLOB ode ode/*.cpp)
source_group(ode FILES ${ode} )

file(GLOB opt opt/*.cpp)
source_group(opt FILES ${opt})


file(GLOB fcn fcn/*.cpp)
source_group(fcn FILES ${fcn})

file(GLOB data data/*.cpp)
source_group(data FILES ${data})

file(GLOB fitting fitting/*.cpp)
source_group(fitting FILES ${fitting})

file(GLOB geometry geometry/*.cpp)
source_group(geometry FILES ${geometry})

file(GLOB proto proto/*.cpp)
source_group(proto FILES ${proto})

add_executable(test-math main.cpp
${global}
${algebra}
${ode}
${root}
${opt}
${fcn}
${data}
${geometry}
${fitting}
${proto}
)

target_link_libraries(test-math yack-math yack-lua++ yack-lua)
yack_link_libraries(test-math)

yack_testing(math)

enable_testing()

#global
add_test(numeric    test-math numeric)
add_test(triplet    test-math triplet)
add_test(adder      test-math adder)
add_test(multiplier test-math multiplier)

#algebra
add_test(iota        test-math iota)
add_test(crout       test-math crout)
add_test(lss         test-math lss)
add_test(jacobi      test-math jacobi)
add_test(svd         test-math svd)
add_test(tridiag     test-math tridiag)
add_test(cyclic      test-math cyclic)
add_test(diagonalize test-math diagonalize)
add_test(cholesky    test-math cholesky)
add_test(ortho       test-math ortho)

#ode
add_test(rk4      test-math rk4)
add_test(explicit test-math explicit)
add_test(app      test-math app)

#root
add_test(zroot  test-math zroot)
add_test(zircon test-math zircon)

#opt
add_test(locate   test-math locate)
add_test(optimize test-math optimize)

#fcn
add_test(drvs      test-math drvs)
add_test(fjac      test-math fjac)
add_test(functions test-math functions)

#data
add_test(poly      test-math poly)
add_test(chebyshev test-math chebyshev)
add_test(corr      test-math corr)
add_test(distrib   test-math distrib)
add_test(linear    test-math linear)

#geom
add_test(area2d test-math area2d)

#fitting
add_test(fitting:lambda  test-math fitting_lambda)
add_test(fitting:vars    test-math fitting_vars)
add_test(fitting:sample  test-math fitting_sample)
add_test(fitting:samples test-math fitting_samples)
add_test(fitting:gls     test-math fitting_gls)
add_test(fitting:ode     test-math fitting_ode)
add_test(fitting:ode2    test-math fitting_ode2)

add_test(proto:diff1d    test-math proto_diff1d)


