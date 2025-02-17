^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package status_monitor
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.6.8 (2023-05-09)
------------------
* Merge remote-tracking branch 'origin/master' into clean-dicts
* Merge branch 'master' into feature/noise_delay_object
* Merge remote-tracking branch 'origin/master' into emergency-state/backwardcompatibility-1
* Merge remote-tracking branch 'origin/master' into feature/add_setgoalposes_api
* Merge branch 'master' into fix/cleanup_code
* Merge remote-tracking branch 'origin/master' into feature/interpreter/license_and_properties
* Merge remote-tracking branch 'origin/master' into fix/get-unique-route-lanelets
* Merge remote-tracking branch 'origin/master' into feature/interpreter/user-defined-value-condition
* Merge pull request `#932 <https://github.com/tier4/scenario_simulator_v2/issues/932>`_ from tier4/feature/interpreter/alive-monitoring
* Update `status_monitor` to output status as filename `<node-name>_status.json`
* Cleanup
* Remove `status_monitor` from `openscenario_interpreter_node`
* Add new member function `StatusMonitor::write`
* Lipsticks
* Update `StatusMonitor` to monitor (min|max)imum access interval
* Update struct `Status` to hold first caller name
* Rename output filename to `<nodename>_status`
* Update `StatusMonitor` to manage whether the monitoring thread has exited or not
* Update `StatusMonitor` to output status as JSON
* Update `StatusMonitor` to check elapsed time since last access
* Fix some CI errors
* Add experimental package `status_monitor`
* Contributors: Dawid Moszyński, Kotaro Yoshimoto, Michał Kiełczykowski, Shota Minami, hrjp, kyoichi-sugahara, yamacir-kit
