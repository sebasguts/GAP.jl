## Converters

from_gap(obj::Int64) = obj
from_gap(obj::Int64,::Any) = obj
from_gap(obj::MPtr) = from_gap(obj,Any)
from_gap(obj::MPtr,::Type{Symbol}) =  Symbol(from_gap(obj))
from_gap(obj::GAPObj,::Type{GAPObj}) = obj

function from_gap( obj :: MPtr, ::Type{Array{GAPObj,1}} )
    len_list = length(obj)
    new_array = Array{GAPObj,1}( undef, len_list)
    for i in 1:len_list
        new_array[ i ] = obj[i]
    end
    return new_array
end

function from_gap( obj :: MPtr, ::Type{Array{T,1}} ) where T
    len_list = length(obj)
    new_array = Array{T,1}( undef, len_list)
    for i in 1:len_list
        new_array[ i ] = from_gap(obj[i],T)
    end
    return new_array
end

function from_gap( obj :: MPtr, ::Type{Dict{Symbol,T}} ) where T
    if ! Globals.IsRecord( obj )
        throw(ArgumentError("first argument is not a record"))
    end
    names = Globals.RecNames( obj )
    names_list = from_gap(names,Array{Symbol,1})
    dict = Dict{Symbol,T}()
    for i in names_list
        dict[ i ] = from_gap(getproperty(obj,i),T)
    end
    return dict
end
