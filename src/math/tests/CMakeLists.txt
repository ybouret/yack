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

file(GLOB fit fit/*.cpp)
source_group(fit FILES ${fit})

file(GLOB geometry geometry/*.cpp)
source_group(geometry FILES ${geometry})


add_executable(test-math main.cpp
${global}
${algebra}
${ode}
${root}
${opt}
${fcn}
${data}
${fit}
${geometry}
)

target_link_libraries(test-math yack-math yack-lua++ yack-lua)
yack_link_libraries(test-math)

yack_testing(math)

enable_testing()

add_test(numeric    test-math numeric)
add_test(triplet    test-math triplet)
add_test(adder      test-math adder)
add_test(multiplier test-math multiplier)

add_test(algebra:iota    test-math iota)
add_test(algebra:lu      test-math lu)
add_test(algebra:jacobi  test-math jacobi)
add_test(algebra:svd     test-math svd)
add_test(algebra:tridiag test-math tridiag)
add_test(algebra:cyclic  test-math cyclic)
add_test(algebra:diag    test-math diagonalize)

add_test(ode:rk4 test-math rk4)
add_test(ode:explicit test-math explicit)
add_test(ode:app      test-math app)

add_test(zroot  test-math zroot)
add_test(zircon test-math zircon)

add_test(optimize test-math optimize)

add_test(drvs     test-math drvs)
add_test(fjac     test-math fjac)
add_test(functions test-math functions)

add_test(data:poly      test-math poly)
add_test(data:chebyshev test-math chebyshev)
add_test(data:corr      test-math corr)
add_test(data:distrib   test-math distrib)

add_test(fit:lam test-math fit_lam)
add_test(fit test-math fit)
add_test(gls test-math gls)

add_test(geom:area2d test-math area2d)

