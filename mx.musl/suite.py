# Dummy mx suite definition, so the CI from https://github.com/oracle/graal can import it.
# See https://github.com/oracle/graal/blob/master/sandbox-toolchains/mx.sandbox-toolchains/suite.py
suite = {
    "mxversion": "7.68.9",
    "name" : "musl",
    "versionConflictResolution" : "latest",
    "capture_suite_commit_info": False,

    "imports" : {
    }
}
