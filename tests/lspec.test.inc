// This should let us run this anywhere and still properly import the file
local(path_here) = io_file_getcwd + '/' + {}->callsite_file->stripLastComponent
not #path_here->endsWith('/') ? #path_here += '/'
local(f) = file(#path_here + '../lspec.inc')
sourcefile(#f->readString, #f->path, false, false)->invoke

// [try] method developed by Bil Corey
define try => {
    local(e) = pair(0='No error')
    local(gb) = givenBlock
    protect => {
        handle_error => { #e = pair(error_code = error_msg) }
        #gB()
    }
    #e->name != 0 ? fail(#e->name,#e->value)
}

// Resets lspec for testing
define lspec_setupForTesting => {
    var(_lspec) = void
    lspec->resultsFormat = 'd'
}


// Runs the various test files
local(f) = file(#path_here + 'lspec_core.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke

local(f) = file(#path_here + 'lspec_testGroup.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke