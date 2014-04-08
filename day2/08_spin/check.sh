
perf record -e sched:* -g -s -o ./perf.data.raw ./a.out
perf inject -v -s -i ./perf.data.raw -o ./perf.data
perf timechart
perf report --stdio --show-total-period -i ./perf.data
