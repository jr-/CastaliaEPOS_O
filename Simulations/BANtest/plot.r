protocols <- c("TSTP", "AODV", "AODVT", "LEACH", "REL", "LABILE")

dataTSTP <- read.csv2("TSTP-txed.txt", header = FALSE)
dataAODV <- read.csv2("AODV-txed.txt", header = FALSE)
dataAODVT <- read.csv2("AODVT-txed.txt", header = FALSE)
dataLEACH <- read.csv2("LEACH-txed.txt", header = FALSE)
dataREL <- read.csv2("REL-txed.txt", header = FALSE)
dataLABILE <- read.csv2("LABILE-txed.txt", header = FALSE)

counts <- t(cbind(dataTSTP, dataAODV, dataAODVT, dataLEACH, dataREL, dataLABILE))
rownames(counts) <- protocols
pdf("plot.pdf")
barplot(counts, main="PHY TX", ylab="TXed packets",
        xlab="Node", #col=c("darkblue","red","green","blue"),
        names.arg=seq(ncol(counts)),
        legend = rownames(counts), beside=TRUE)
dev.off()

counts
