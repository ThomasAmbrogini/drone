using GLMakie
using GeometryBasics
using StaticArrays

Base.@kwdef struct AirplaneMeasure
    wing_w::Float32
    wing_h::Float32
    tailwing_w::Float32
    tailwing_h::Float32
    tail_h::Float32
    fuse_l2::Float32
    fuse_l3::Float32
    head_length::Float32
    head_width::Float32
end

function rotate_mesh(mesh, rotation_matrix)
    rotated_coord = [Point3f(rotation_matrix * v) for v in coordinates(mesh)]
    rotated_mesh = GeometryBasics.mesh(rotated_coord, faces(mesh))
    return rotated_mesh
end

function translate_mesh(mesh, translation_matrix)
    translated_coord = [Point3f(translation_matrix + v) for v in coordinates(mesh)]
    translated_mesh = GeometryBasics.mesh(translated_coord, faces(mesh))
    return translated_mesh
end

function create_airplane_parts(measure::AirplaneMeasure)
    head = GeometryBasics.Pyramid(Point3f(0, 0, measure.fuse_l2), measure.head_length, measure.head_width)
    rotation_head = @SMatrix [
        0 0 1;
        0 1 0;
        1 0 0;
    ]
    head_rotated_mesh = rotate_mesh(GeometryBasics.mesh(head), rotation_head)

    wing = GeometryBasics.mesh(Rect3f(Vec3f(-measure.wing_h, -measure.wing_w/2, 0), Vec3f(measure.wing_h, measure.wing_w, 0)))
    tailwing = GeometryBasics.mesh(Rect3f(Vec3f(-measure.fuse_l3, -measure.tailwing_w/2, 0), Vec3f(measure.tailwing_h, measure.tailwing_w, 0f0)))

    body = GeometryBasics.Pyramid(Point3f(0, 0, -measure.fuse_l2), measure.fuse_l2 + measure.fuse_l3, measure.head_width)
    rotation_body = @SMatrix [
        0 0 -1;
        0 -1 0 ;
        -1 0 0;
    ]
    body_rotated_mesh = rotate_mesh(GeometryBasics.mesh(body), rotation_body)

    vertical_stabilizer_vertices = @SVector [
        Point3f(-(measure.fuse_l3 - measure.tailwing_h), 0, 0),
        Point3f(-measure.fuse_l3, 0, 0),
        Point3f(-measure.fuse_l3, 0, measure.tail_h),
    ]

    vertical_stabilizer_faces = @SVector [
        TriangleFace(1, 2, 3)
    ]

    vertical_stabilizer_mesh = GeometryBasics.mesh(vertical_stabilizer_vertices, vertical_stabilizer_faces)

    return head_rotated_mesh, wing, tailwing, body_rotated_mesh, vertical_stabilizer_mesh 
end

function main()
    fig = Figure()

    ax = Axis3(
        fig[1,1],
        limits=(-20, 20, -20, 20, -10, 10),
    )

    fuse_l2 = 3f0
    wing_h = 3f0
    tailwing_h = 2f0

    airplane_measure = AirplaneMeasure(
        wing_w = 8f0,
        wing_h = wing_h,
        tailwing_w = 4f0,
        tailwing_h = tailwing_h,
        tail_h = 3f0,
        fuse_l2 = fuse_l2,
        fuse_l3 = wing_h + tailwing_h + 4f0,
        head_length = 3f0,
        head_width = 2f0,
    )

    grid_translation = GridLayout(fig[2,1])
    Label(grid_translation[0,:], "Translation Control", fontsize=20, tellwidth=false)

    control_translation_slider_grid = SliderGrid(
        grid_translation[1,1],
        (label="Translate along X", range=-10:0.1:10, startvalue=0f0),
        (label="Translate along Y", range=-10:0.1:10, startvalue=0f0),
        (label="Translate along Z", range=-10:0.1:10, startvalue=0f0),
    )

    translate_x = control_translation_slider_grid.sliders[1].value
    translate_y = control_translation_slider_grid.sliders[2].value
    translate_z = control_translation_slider_grid.sliders[3].value

    grid_rotation = GridLayout(fig[3,1])
    Label(grid_rotation[0,:], "Rotation Control", fontsize=20, tellwidth=false)
    control_rotation_slider_grid = SliderGrid(
        grid_rotation[1,1],
        (label="Yaw Rotation", range=-90:0.1:90, startvalue=0f0),
        (label="Pitch Rotation", range=-90:0.1:90, startvalue=0f0),
        (label="Roll rotation", range=-90:0.1:90, startvalue=0f0),
    )

    yaw_angle = control_rotation_slider_grid.sliders[1].value
    pitch_angle = control_rotation_slider_grid.sliders[2].value
    roll_angle = control_rotation_slider_grid.sliders[3].value

    airplane_parts = create_airplane_parts(airplane_measure)
    full_rotation_matrix = @lift begin
        yaw_rotation_matrix = @SMatrix [
            cosd($yaw_angle) -sind($yaw_angle) 0;
            sind($yaw_angle) cosd($yaw_angle) 0;
            0 0 1;
        ]

        roll_rotation_matrix = @SMatrix [
            1 0 0;
            0 cosd($roll_angle) sind($roll_angle);
            0 -sind($roll_angle) cosd($roll_angle);
        ]

        pitch_rotation_matrix = @SMatrix [
            cosd($pitch_angle) 0 -sind($pitch_angle);
            0 1 0;
            sind($pitch_angle) 0 cosd($pitch_angle);
        ]

        yaw_rotation_matrix * pitch_rotation_matrix * roll_rotation_matrix
    end

    translation_matrix = @lift begin
        @SVector [
            $translate_x;
            $translate_y;
            $translate_z;
        ]
    end

    for part in airplane_parts
        transformed_part = @lift begin
            rotated_part = rotate_mesh(part, $full_rotation_matrix)
            translated_part = translate_mesh(rotated_part, $translation_matrix)
        end

        mesh!(
            ax,
            transformed_part,
        )
    end

    display(fig)
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end

