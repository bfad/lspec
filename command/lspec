#!/usr/bin/env lasso9

define usage(exit_status::integer=-1) => {
    local(cmd) = $argv->get(1)->lastComponent
    stdoutnl("\
usage: " + #cmd + " [options] [file or directory paths]

    The " + #cmd + " command executes the Lasso code in the specified file or
    all Lasso files in the specified directory. If no file or directory path is 
    specified, then it tries a relative directory named \"spec\".
    
    When a directory is specified (or when using the default \"spec\" directory),
    the command executes all files ending in \".spec.inc\" in that directory. Then 
    it proceeds to do the same thing in any nested directories. (To specify what
    extensions are considered Lasso files, see the \"-exts\" option.)

Options
-------
    -documentation
        Output just the group and test descriptions.
    
    -exts CSV_EXT_LIST
        Allows for specifying a comma-separated list of file extensions to be
        considered Lasso files. This is useful when specifying a directory - all
        files ending in .spec.[listed extensions] will be run.

    -nocolor
        Don't use terminal colors for the output.
        
    -h
        Displays this help and exits."
    )
    sys_exit(#exit_status)
}

define lspec_runDir(d::dir, ext::staticarray=(:'inc')) => {
    with f in #d->eachFile
    let test = #f->path->split('.')
    where #test->size > 0
    where #ext->contains(#test->last)
    where 'spec' == #test->get(#test->size - 1)
    do lspec_runFile(#f)
    
    with f in #d->eachDir do lspec_runDir(#f, #ext)
}

define lspec_runFile(f::file) => {
    sourcefile(#f, -autoCollect=false)->invoke
}

// Defaults
local(exts)      = (:'inc')
local(paths)     = array
local(mode)      = 'p'
local(use_color) = true

local(opts) = ($argv->asArray->remove(1,1)&)
while(#opts->size > 0) => {
    match(bytes(#opts(1))) => {
    case(bytes('-h'))
        usage(0)
        
    case(bytes('-documentation'))
        #mode = 'd'

    case(bytes('-nocolor'))
        #use_color = false
        
    case(bytes('-exts'))
        #opts->size < 2
            ? usage
        
        #exts = #opts->get(2)->split(',')->asStaticArray
        // Since we're dealing with the next item, delete one now (we delete the other later)
        #opts->remove(1,1)
        
    case
        #paths->insert(#opts(1))
    }
    
    #opts->remove(1,1)
}
#paths->size == 0
    ? #paths->insert('./spec/')

lspec->formatResults(#mode)

#use_color? lspec->useColor

with path in #paths 
let f = file(#path)
do {
    if(not #f->exists) => {
        file_stderr->writeString(error_code_resNotFound + ':' + error_msg_resNotFound + ' - ' + #path + sys_eol)
        sys_exit(1)
    }
    #f->isDir? lspec_runDir(dir(#path), #exts)
             | lspec_runFile(#f)
}

lspec->stop
