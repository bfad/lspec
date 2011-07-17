// This should let us run this anywhere and still properly import the file
local(path_here) = io_file_getcwd + '/' + {}->callsite_file->stripLastComponent
not #path_here->endsWith('/') ? #path_here += '/'
local(f) = file(#path_here + '../lspec.inc')
sourcefile(#f->readString, #f->path, false, false)->invoke

local(test_desc) = "It should throw an error if an [it] method isn't inside a [describe] method"

try => {
    local(success) = false
    handle(error_code == error_code_lspecNoSurroundingDescribe &&
           error_msg  == error_msg_lspecNoSurroundingDescribe) => 
    {
        failure_clear
        error_reset
        #success = true
    }
    
    it("does something") => {}
    if(not #success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

    #success = false
    describe("Something") => {
        it("does something") => {}
        #success = true
    }
    if(not #success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
    


#test_desc = "An [it] method should only have it's givenBlock called by a [describe] method"
#success = false
local(gets_to_it) = false
describe("Something") => {
    it("does something") => {
        #success = true
    }
    if(#success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}
if(not #success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}