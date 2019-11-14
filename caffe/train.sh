#!/usr/bin/env sh
build/tools/caffe train --solver=examples/MobileNet_v2_tsr/solver.prototxt --model=models/MobileNet_v2_tsr/mobilenet_v2.caffemodel --gpu all 2>&1|tee  models/MobileNet_v2_tsr/logs/MobileNet_v2_tsr.log
#####--snapshot=models/MobileNet_v/MobileNet_v2_SE_iter_440000.solverstate 

