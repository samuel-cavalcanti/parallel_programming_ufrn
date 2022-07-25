#!/bin/Rscript

#algorithm time process
t = read.table('data.dat',header=TRUE)

t["time"] = t["time"] *1e6

library("ggplot2")
plot <- ggplot(t, aes(process,time,colour=algorithm)) +ylab("time in microseconds") + xlab("number of process") + geom_point() + geom_smooth(method=lm)
ggsave("butterfly_vs_ring_pass.svg")


