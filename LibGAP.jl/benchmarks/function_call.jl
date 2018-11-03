using BenchmarkTools

include( joinpath( "..", "test", "runtests_dir.jl" ) )

include(INIT_FILE)

libgap.run_it(GAPPATH)

function test_call(range)
    x = 1:range
    y = 1:range
    map(GAP.GAPFuncs.ReturnFirst,x,y)
end

function test_call_cached(range)
    x = 1:range
    y = 1:range
    ret_first = GAP.GAPFuncs.ReturnFirst
    map(ret_first,x,y)
end

function test_call_pure(range)
    x = 1:range
    y = 1:range
    ret_first = (i,j) -> i
    map(ret_first,x,y)
end

range = 10000

@btime test_call($range)
@btime test_call_cached($range)
@btime test_call_pure($range)