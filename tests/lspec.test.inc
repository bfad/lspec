// This should let us run this anywhere and still properly import the file
local(path_here) = io_file_getcwd + '/' + {}->callsite_file->stripLastComponent
not #path_here->endsWith('/') ? #path_here += '/'
local(f) = file(#path_here + '../lspec.inc')
sourcefile(#f->readString, #f->path, false, false)->invoke

// [try] method: Based on code from Bil Corey
define try => {
    local(err) = null
    local(gb)  = givenBlock
    protect => {
        handle_error => { #err = (: error_code, error_msg, error_stack) }
        #gB()
    }
    null !== #err? fail(#err->get(1), #err->get(2), #err->get(3))
}

// Resets lspec for testing
define lspec_setupForTesting => {
    var(_lspec) = void
    lspec->formatResults('d')
    lspec->suppressOutput
}


// Runs the various test files
local(f) = file(#path_here + 'lspec_testGroup.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke

local(f) = file(#path_here + 'lspec_testSuite.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke

local(f) = file(#path_here + 'lspec_core.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke

local(f) = file(#path_here + 'lspec_output_d.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke

local(f) = file(#path_here + 'lspec_output_p.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke

local(f) = file(#path_here + 'lspec_expectations.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke

local(f) = file(#path_here + 'lspec_local_scope.test.inc')
sourcefile(#f->readString, #f->path, true, false)->invoke