// This should let us run this anywhere and still properly import the file
local(path_here) = io_file_getcwd + '/' + {}->callsite_file->stripLastComponent
not #path_here->endsWith('/') ? #path_here += '/'
local(f) = file(#path_here + '../lspec.inc')
sourcefile(#f->readString, #f->path, false, false)->invoke

local(test_desc) = "Given a describe with 2 it blocks, it should return 2 as the number of tests"
lspec_clearSuite
describe("Something") => {
    it("Does something") => {}
    it("Does something else") => {}
}
if(lspec->suite->first->numTests != 2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
