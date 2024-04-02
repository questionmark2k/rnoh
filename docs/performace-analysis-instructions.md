# Performance analysis using SmartPerf-Host instructions

Performance of ArkUI applications can be analysed using SmartPerf-Host tool, which can be downloaded from https://gitee.com/openharmony/developtools_smartperf_host/releases. 

To run trace on an tester application do the following
- download and unpack SmartPerf from the link above
- open the app
- run `npm run hiprofiler` to start tracing in the tester directory - the trace log will be saved in `hiprofiler_data.htrace` in the tester
- launch `./main_darwin` from the unpacked SmartPerf directory
- open https://localhost:9000/application/ in your web browser to launch SmartPerf tool - ignore warnings about the page being not secure
- open the trace file using the tool
- the trace decomposition should now be visible in the browser

## Adding custom trace sections
- you can add custom `systrace` sections to the cpp files by creating `facebook::react::SystraceSection` in the method that you want to analyse - it's trace will be then visible in the `SmartPerf` tool under the `com.rnoh.tester` tab
```cpp
//systrace example
void Component::onUpdate() {
  facebook::react::SystraceSection s("Component::onUpdate", ...args);
  ...
}
``` 

## hiprofiler config
To analyse ArkUI builtin functionalites (for example rendering and layouting) you can use the more detailed profiler configuration visible below. To use this config paste it into `scripts/hiprofiler-config.txt` in the tester dir.

```
request_id: 1
session_config {
  buffers {
    pages: 16384
  }
}
plugin_configs {
  plugin_name: "ftrace-plugin"
  sample_interval: 0
  config_data {
    ftrace_events: "sched/sched_switch"
    ftrace_events: "power/suspend_resume"
    ftrace_events: "sched/sched_wakeup"
    ftrace_events: "sched/sched_wakeup_new"
    ftrace_events: "sched/sched_waking"
    ftrace_events: "sched/sched_process_exit"
    ftrace_events: "sched/sched_process_free"
    ftrace_events: "task/task_newtask"
    ftrace_events: "task/task_rename"
    ftrace_events: "power/cpu_frequency"
    ftrace_events: "power/cpu_idle"
    hitrace_categories: "ability"
    hitrace_categories: "ace"
    hitrace_categories: "app"
    hitrace_categories: "ark"
    hitrace_categories: "binder"
    hitrace_categories: "disk"
    hitrace_categories: "freq"
    hitrace_categories: "graphic"
    hitrace_categories: "idle"
    hitrace_categories: "irq"
    hitrace_categories: "memreclaim"
    hitrace_categories: "mmc"
    hitrace_categories: "multimodalinput"
    hitrace_categories: "ohos"
    hitrace_categories: "pagecache"
    hitrace_categories: "rpc"
    hitrace_categories: "sched"
    hitrace_categories: "sync"
    hitrace_categories: "window"
    hitrace_categories: "workq"
    hitrace_categories: "zaudio"
    hitrace_categories: "zcamera"
    hitrace_categories: "zimage"
    hitrace_categories: "zmedia"
    buffer_size_kb: 20480
    flush_interval_ms: 1000
    flush_threshold_kb: 4096
    parse_ksyms: true
    clock: "boot"
    trace_period_ms: 200
    debug_on: false
  }
}
```

## Further details
- [Common trace usage instructions](https://docs.openharmony.cn/pages/v4.0/zh-cn/application-dev/performance/common-trace-using-instructions.md)
- [Analyzing application performance using SmartPerf-Host](https://docs.openharmony.cn/pages/v4.0/zh-cn/application-dev/performance/performance-optimization-using-smartperf-host.md)