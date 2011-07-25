local(test_desc) = "It should throw an error if an [it] method isn't inside a [describe] method"
lspec_setupForTesting
try => {
    local(success) = false
    handle(error_code == error_code_lspecNotInTestGroup &&
           error_msg  == error_msg_lspecNotInTestGroup) => 
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
lspec_setupForTesting
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
lspec_setupForTesting
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
lspec_setupForTesting
describe("Something") => {}
if(lspec->result != "Something") => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [lspec_clearSuite] clears the suite and the result but leaves options"
// Completely reseting the lspec alias
var_defined('_lspec') ? $_lspec = void
lspec->resultsFormat = 'd'
lspec->indentor       = ' '
lspec->suppressOutput = true
describe("Something") => {}
lspec_clearSuite
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
if(lspec->resultsFormat != 'd' || lspec->indentor != ' ') => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
lspec_setupForTesting



#test_desc = "Testing documentation output of a pending test & [lspec->numTests] & [lspec->numPending] increments while [lspec->numSuccesses] does not"
lspec_setupForTesting
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
if(lspec->numSuccesses != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if an [expect] method isn't inside an [it] method"
lspec_setupForTesting
try => {
    local(success) = false
    handle(error_code == error_code_lspecNotInTest &&
           error_msg  == error_msg_lspecNotInTest) => 
    {
        failure_clear
        error_reset
        #success = true
    }
    
    expect(true)
    if(not #success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

#success = false
describe("Something") => {
    it("does something") => {
        expect(true)
        #success = true
    }
}
if(not #success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Increments [lspec->numSuccesses] & [lspec->numTestCases] and has the proper documentation output"
lspec_setupForTesting
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
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Tests for valid resultsFormat options"
lspec_setupForTesting
try => {
    local(success) = false
    handle(error_code == error_code_lspecInvalidResultFormat &&
           error_msg  == error_msg_lspecInvalidResultFormat) => 
    {
        failure_clear
        error_reset
        #success = true
    }
    
    lspec->resultsFormat = '#'
    if(not #success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}
lspec->resultsFormat = 'p'
lspec->resultsFormat = 'd'



#test_desc = "Output is properly suppressed"
lspec_setupForTesting
// save stdout using ->dup
local(saveStdout = file_stdout->fd->dup)
// create a local conduit
local(readFd, writeFd) = io_file_pipe
// close real stout, reopen it on our write pipe using dup2 system call
#writeFd->dup2(file_stdout->fd)
// make a file object here which we read stout from
local(readTest = file(#readFd))

describe("Something") => {
    it("does something") => {}
}

// read test data
local(didRead = #readTest->readString)
// set stout back using our save
#saveStdout->dup2(file_stdout->fd)

if(#didRead != '') => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Output is properly not suppressed"
lspec_setupForTesting
lspec->suppressOutput = false
// save stdout using ->dup
local(saveStdout = file_stdout->fd->dup)
// create a local conduit
local(readFd, writeFd) = io_file_pipe
// close real stout, reopen it on our write pipe using dup2 system call
#writeFd->dup2(file_stdout->fd)
// make a file object here which we read stout from
local(readTest = file(#readFd))

describe("Something") => {
    it("does something") => {}
}

// read test data
local(didRead = #readTest->readString)
// set stout back using our save
#saveStdout->dup2(file_stdout->fd)

if(#didRead != lspec->result) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}