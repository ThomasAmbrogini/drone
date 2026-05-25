using LinearAlgebra

function rotation_matrix()

end

function create_rotation_matrix(angle_rotation)
  [cos(angle_rotation)  -sin(angle_rotation);
   sin(angle_rotation)   cos(angle_rotation)]
end

v = [1.0, 0.0]

angle_rotation = pi/2
R = create_rotation_matrix(angle_rotation)

v_rotated = R * v

println("Original vector: ", v)
println("Rotated vector:  ", v_rotated)

println("Dot product: ", dot(v, v_rotated))

println("Original magnitude: ", norm(v))
println("Rotated magnitude:  ", norm(v_rotated))
