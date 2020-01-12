set term png
set xrange [0.0:1.0]
set yrange [0.0:0.3]

do for [n = 2:202] {
    set output sprintf("step_%03d.png", n-2)
    plot 'build/out.data' skip 1 using 1:n with linespoints
}
! convert -delay 3 step_*.png -loop 0 hat.gif
! rm step_*.png
