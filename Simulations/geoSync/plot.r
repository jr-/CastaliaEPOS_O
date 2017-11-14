# Parse simulation scenarios
fileList <- sort(list.files(pattern="^[^-]*-scenarios.csv"))
for (scenario in fileList) {
    configName <- gsub("-.*", "", gsub("-scenarios.csv", "", scenario))
    fullData <- read.csv(scenario)
    # Remove leading 'X.' from column names, if present
    colnames(fullData) <- gsub("^X.", "", colnames(fullData))
    # Order by runNumber
    fullData <- fullData[with(fullData, order(runNumber)), ]

    scenarios <- fullData

    # Find column with the most unique values (excluding runNumber). This will be our x axis
    data <- scenarios[, !(colnames(scenarios) %in% "runNumber")]
    idx <- 0
    lmax <- 0
    for (column in data) {
        idx <- idx + 1
        l <- length(unique(column))
        if (l > lmax) {
            x <- data[idx]
            lmax <- l
        }
    }
    # Everything that is not in x axis or runNumber
    notx <- data[!(colnames(data) %in% names(x))]

    # Our lines will be a concatenation of all 'notx' values
    data <- notx
    lines <- data[,1]
    data <- data[-1]
    for (i in data) {
        lines <- paste(lines,data[,1])
        data <- data[-1]
    }
    # Correlation between line name, x axis value, and runNumber
    data <- cbind(lines, x, scenarios$runNumber)
    fullData <- cbind(fullData, lines)
    colnames(data)[ncol(data)] <- "runNumber"
    colnames(data)[colnames(data) == "lines"] <- "line"
    colnames(fullData)[colnames(fullData) == "lines"] <- "line"
    # The names of lines to be plotted
    lines <- unique(lines)

    run <- scenarios$runNumber[1]
    tmp <- read.csv(paste(configName, "-", run, "-data.csv", sep=""))
    colnames(tmp) <- gsub("^X.", "", colnames(tmp))
    idx <- 0
    for (col in tmp) {
        idx <- idx + 1
        addcol <- data.frame(seq(length(scenarios$runNumber)))
        addcol[] <- NA
        colnames(addcol)[1] <- colnames(tmp)[idx]
        fullData <- cbind(fullData, addcol)
    }

    dataFiles <- list.files(pattern=paste(configName, "-[0-9]*-data.csv", sep=""))
    for (run in scenarios$runNumber) {
        fileName <- paste(configName, "-", run, "-data.csv", sep="")
        if (length(which(dataFiles == fileName)) > 0) {
            idx <- 0
            tmp <- read.csv(fileName)
            for (col in tmp) {
                idx <- idx + 1
                fullData[fullData$runNumber == run, colnames(fullData) %in% names(tmp[idx])] <- mean(col)
            }
        }
    }

    write.csv(fullData, file=paste(configName, "-data.csv", sep=""))

    startColIdx <- 1
    for (i in 1:ncol(fullData)) {
         if (colnames(fullData)[i] == "line") {
             startColIdx <- i + 1
         }
    }

    for (colIdx in seq(startColIdx, ncol(fullData))) {
        if (colnames(fullData)[colIdx] != "runNumber" && colnames(fullData)[colIdx] != colnames(x)) {

            pdfName = paste("plot-", configName, "-", colnames(fullData)[colIdx], ".pdf", sep="")
            print(pdfName)
            pdf(pdfName)

            style <- 1
            xAxis <- unique(x)
            xrange <- range(as.numeric(xAxis[,1]))
            yrange <- range(fullData[!is.na(fullData[colIdx]), colIdx])
            par(mar=c(5.1, 4.1, 4.1, 8.1), xpd=TRUE)
            plot(xrange, yrange, xaxt="n", type="n", ylab=colnames(fullData)[colIdx], xlab=colnames(xAxis))
            if(is.numeric(xAxis[,1])) {
                axis(1, at=xAxis[,1])
            } else {
                axis(1, at=seq(length(xAxis[,1])), lab=xAxis[,1])
            }
            grid()

            for (line in lines) {
                subset <- fullData[fullData$line == line, ]
                xAxisNames <- subset[, colnames(subset) %in% names(x)]
                if(is.numeric(xAxisNames)) {
                    xAxis <- xAxisNames
                } else {
                    xAxis <- as.numeric(xAxisNames)
                    xAxis <- xAxis[order(xAxis)]
                }
                #axis(1, at=seq_along(xAxis),labels=r), las=2)

                lines(xAxis, subset[,colIdx], type="o", pch=style, lty=style, col=style)
                style <- style + 1
            }
            data <- colnames(notx)
            legend <- data[1]
            data <- data[-1]
            for (i in data) {
                legend <- paste(legend,data[1])
                data <- data[-1]
            }
            title(c(configName, legend))
            legend(par('usr')[2], par('usr')[4], bty='n', cex=0.8, lines, col=seq(style), pch=seq(style), lty=seq(style))

            dev.off()
        }
    }
}
