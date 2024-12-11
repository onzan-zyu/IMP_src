#### command

```
command:/morpher/hycube_simulator/src/build/hycube_simulator -x 4 -y 4 -c /morpher/hycube_simulator/benchmarks/morpher_benchmarks/array_add/*.bin -d /morpher/Morpher_DFG_Generator/benchmarks/morpher_benchmarks/array_add/memtraces/array_add_trace_0.txt -a /morpher/hycube_simulator/benchmarks/morpher_benchmarks/array_add/array_add_mem_alloc.txt -m 4096
```
hycube_simulator -x 4 -y 4  -c ./sourcefile/*.bin  -d ./sourcefile/array_add_trace_0.txt -a ./sourcefile/array_add_mem_alloc.txt -m 4096

config mem file: array_add_PartPredDFG.xml_DP1_XDim=4_YDim=4_II=4_MTP=1_binary.bin

mem alloc file:array_add_mem_alloc.txt

data mem file: array_add_trace_0.txt



模拟SPM的运行