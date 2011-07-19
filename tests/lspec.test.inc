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



#test_desc = "A [describe] statement inserts a test group object into [lspec->suite]"
local(num_before) = lspec->suite->size
describe("Something") => {}
if(lspec->suite->size != (#num_before + 1)) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(not lspec->suite->get(#num_before + 1)->isA(::lspec_testGroup)) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "A [describe] statement inserts into [lspec->result]"
// Completely reseting the lspec alias
var_defined('_lspec') ? $_lspec = void
describe("Something") => {}
if(lspec->result != "Something") => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
var_defined('_lspec') ? $_lspec = void



#test_desc = "The [lspec->clearSuite] clears the suite and the result"
// Completely reseting the lspec alias
var_defined('_lspec') ? $_lspec = void
describe("Something") => {}
lspec->clearSuite
if(lspec->suite->size != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->result != '') => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
var_defined('_lspec') ? $_lspec = void



#test_desc = "Testing the basic output of a successful test & [lspec->numTests] & [lspec->numPending] increments"
lspec->clearSuite
lspec->printSuccesses = true
describe("Something") => {
    it("does something") => {}
}
if(lspec->result != 'Something\n' + lspec->indentor + 'does something') => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Increments [lspec->numSuccesses] & [lspec->numTestCases] and has the proper success output"
lspec->clearSuite
lspec->printSuccesses = true
describe("Something") => {
    it("does something") => {
        expect(true)
    }
}
if(lspec->result != 'Something\n' + lspec->indentor + 'does something') => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTestCases != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}