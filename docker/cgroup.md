$ mount -t cgroup
cgroup on /sys/fs/cgroup/cpuset type cgroup (rw,relatime,cpuset)
cgroup on /sys/fs/cgroup/cpu type cgroup (rw,relatime,cpu)
cgroup on /sys/fs/cgroup/cpuacct type cgroup (rw,relatime,cpuacct)
cgroup on /sys/fs/cgroup/memory type cgroup (rw,relatime,memory)
cgroup on /sys/fs/cgroup/devices type cgroup (rw,relatime,devices)
cgroup on /sys/fs/cgroup/freezer type cgroup (rw,relatime,freezer)
cgroup on /sys/fs/cgroup/blkio type cgroup (rw,relatime,blkio)
cgroup on /sys/fs/cgroup/net_prio type cgroup (rw,net_prio)
cgroup on /sys/fs/cgroup/net_cls type cgroup (rw,net_cls)
cgroup on /sys/fs/cgroup/perf_event type cgroup (rw,relatime,perf_event)
cgroup on /sys/fs/cgroup/hugetlb type cgroup (rw,relatime,hugetlb)



$ lssubsys  -m
cpuset /sys/fs/cgroup/cpuset
cpu /sys/fs/cgroup/cpu
cpuacct /sys/fs/cgroup/cpuacct
memory /sys/fs/cgroup/memory
devices /sys/fs/cgroup/devices
freezer /sys/fs/cgroup/freezer
blkio /sys/fs/cgroup/blkio
net_cls /sys/fs/cgroup/net_cls
net_prio /sys/fs/cgroup/net_prio
perf_event /sys/fs/cgroup/perf_event
hugetlb /sys/fs/cgroup/hugetlb



$ cd /sys/fs/
$ mkdir cgroup
$ mount -t tmpfs cgroup_root ./cgroup
$ mkdir cgroup/cpuset
$ mount -t cgroup -ocpuset cpuset ./cgroup/cpuset/
$ mkdir cgroup/cpu
$ mount -t cgroup -ocpu cpu ./cgroup/cpu/
$ mkdir cgroup/memory
$ mount -t cgroup -omemory memory ./cgroup/memory/



$ ls /sys/fs/cgroup/cpu /sys/fs/cgroup/cpuset/
/sys/fs/cgroup/cpu:
cgroup.clone_children  cgroup.sane_behavior  cpu.shares         release_agent
cgroup.event_control   cpu.cfs_period_us     cpu.stat           tasks
cgroup.procs           cpu.cfs_quota_us      notify_on_release  user
 
/sys/fs/cgroup/cpuset/:
cgroup.clone_children  cpuset.mem_hardwall             cpuset.sched_load_balance
cgroup.event_control   cpuset.memory_migrate           cpuset.sched_relax_domain_level
cgroup.procs           cpuset.memory_pressure          notify_on_release
cgroup.sane_behavior   cpuset.memory_pressure_enabled  release_agent
cpuset.cpu_exclusive   cpuset.memory_spread_page       tasks
cpuset.cpus            cpuset.memory_spread_slab       user
cpuset.mem_exclusive   cpuset.mems



fengye@ubuntu:/sys/fs/cgroup/cpu$ sudo mkdir haoel
[sudo] password for fengye: 
fengye@ubuntu:/sys/fs/cgroup/cpu$ ls ./haoel
cgroup.clone_children  cgroup.procs       cpu.cfs_quota_us  cpu.stat           tasks
cgroup.event_control   cpu.cfs_period_us  cpu.shares        notify_on_release

