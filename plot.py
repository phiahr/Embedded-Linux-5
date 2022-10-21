import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as mticker  

times_cpp = [0.086, 0.346, 1.644, 5.987, 25.164, 88.217, 333.345, 1019.88]
times_opencl = [0.02, 0.071, 0.264, 1.041, 4.143, 16.71, 66.78, 164.38]

x_axis = [64, 128, 256, 512, 1024, 2048, 4096, 8192]

fig = plt.figure(figsize=(16,4))
ax = fig.add_subplot(1,1,1)



n = 14
a = np.arange(start=6, stop=n)
x = 2**a
x2 = ['64*64', '128*128', '256*256', '512*512', '1024*1024', '2048*2048', '4096*4096', '8192*8192']
# x2 = ['64^2', '128^2', '256^2', '512^2', '1024^2', '2048^2', '4096^2', '8192^2']
print(x)

ax.plot(a,times_cpp, label='vector addition with cpp')
ax.plot(a,times_opencl, label='vector addition with opencl')
ax.set_xlabel('vector size')
ax.set_ylabel('calculation time')
# ax.set_xticks(x_axis)
ax.xaxis.set_ticks(a) #set the ticks to be a
ax.xaxis.set_ticklabels(x2)
plt.gca().yaxis.set_major_formatter(mticker.FormatStrFormatter('%.1f ms'))
# plt.gca().xaxis.set_major_formatter(mticker.FormatStrFormatter('%f x'))
# plt.gca().xaxis.set_major_formatter(mticker.StrMethodFormatter('{x} fs'))


# plt.plot(x_axis, times_cpp)
# plt.plot(x_axis, times_opencl)
plt.legend()
# plt.show()
plt.savefig('plot_diff.pdf')