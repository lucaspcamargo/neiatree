# neiatree
neiatree is an asset tree processing tool for games. Requires Qt 5.4. Change util/nIODefines.h for lower Qt versions.

## RPT_RULES format

First of all, an example is available at docs/Example RPT_RULES. But first, read this.

Basic format is:

`"REGEX_HERE" TASK_TYPE [arg1] [arg2] ... [argn]`

This will select all files for task `TASK_TYPE`.
Files that match more than one regex won't
be processed twice.
In the future, some tasks might process many files
at once, eg. a ZIP task could zip all matching files
in one go.

### Wildcards

Preceed the task type with a W like this:

`W "REGEX_HERE" TASK_TYPE [arg1] [arg2] ... [argn]`

to use simpler wildcard matching instead of regexes.
Using `W "*.*"` to match all files in a directory is very useful.

### Tasks

Task types are:

`COPY` (simple copy)

`COPY_REPLACE` (copy text files while replacing text

`COPY_REGEX_REPLACE` (the same, replaces regexps)

`EXTERNAL_PROCESS` perform a job using an external process, e.g. texture or sound compression

Enter directories recursively with `PROCESS_DIR` task.
Each directory must contain its own "RPT_RULES" file.

Entries named RPT_RULES, Thumbs.db, RPT_CONFIG,
RPT_VERSION_CONTROL, and otherwise hidden files are
excluded from regex match :)

Any line that is empty, or starts with "#" or "//"
will be ignored. Anything before the first quote of
the rule's regex will be ignored.

You can create a task type alias by writing the alias,
preceded by "@@@", instead of a regex,  e.g.:

`"@@@S2TC" EXTERNAL_PROCESS ENV:COLOR_DIST_MODE=sRGB...`

Then you can write:

`"*.tga" S2TC`

And it will mean:

`"*.tga" EXTERNAL_PROCESS ENV:COLOR_DIST_MODE=sRGB...`

This way you can easily create "presets" that you
modify once, to affect many files in an automated
way. **Disabling the version control system or removing
destination files might be necessary, because the vcs
does not know when an alias has been modified.**
