using GLMakie

function draw_rod!(ax, L, teta, g, h, y, w, plot)
    X = [y + w/2, y + w/2 + sin(deg2rad(teta)) * L]
    Y = [g + h, g + h + cos(deg2rad(teta)) * L]
    if plot === nothing
        plot = lines!(ax, X, Y)
    else
        delete!(ax, plot)
        plot = lines!(ax, X, Y)
    end

    return plot
end

function draw_base!(ax, y, w, h, g)
    X = [y, y + w, y + w, y]
    Y = [g, g, g + h, g + h]
    vertices = Observable(Point2f.(X, Y))
    plot = poly!(ax, vertices)
    return vertices
end

function update_base!(vertices, w, h, g)
    X = [0, w, w, 0]
    Y = [g, g, g + h, g + h]
    vertices[] = Point2f.(X, Y)
    notify(vertices)
    return vertices
end

function main()
    fig = Figure()

    ax = Axis(
        fig[1,1],
        xlabel = "x label",
        ylabel = "y label",
        title = "title",
        limits = (0, 20, 0, 20),
        aspect = DataAspect(),
    )
    hidedecorations!(ax)

    teta = 30.0
    L = 10
    g = 1
    h = 4

    w = 8
    y = 3

    rod_plot = nothing
    vertices = base_plot = draw_base!(ax, y, w, h, g)

    i = 0
    while(i < (30 * 5))
        rod_plot = draw_rod!(ax, L, teta, g, h, y, w, rod_plot)
        display(fig)
        sleep(1/30)
        i = i + 1
        teta = teta + 0.1
    end
end

main()

