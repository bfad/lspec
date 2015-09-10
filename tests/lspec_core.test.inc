local(test_desc) = "It should throw an error if an [it] method isn't inside a [describe] method"
lspec_setupForTesting
try => {
    var(success) = false
    handle(error_code == error_code_lspecNotInTestGroup &&
           error_msg  == error_msg_lspecNotInTestGroup) => 
    {
        failure_clear
        error_reset
        $success = true
    }
    
    it("does something") => {}
    if(not $success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

$success = false
describe("Something") => {
    it("does something") => {}
    $success = true
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
    


#test_desc = "It should throw an error if a [describe] method is inside an [it] method"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_lspecTestGroupInTest &&
                   error_msg  == error_msg_lspecTestGroupInTest) => 
            { $success = true }
            describe("Should Fail") => {}
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
        
        
        
#test_desc = "An [it] method should only have it's givenBlock called by a [describe] method"
lspec_setupForTesting
$success = false
local(gets_to_it) = false
describe("Something") => {
    it("does something") => {
        $success = true
    }
    if($success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}
if(not $success) => {
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



#test_desc = "The [lspec_clearSuite] clears the suite and the result but leaves options"
// Completely reseting the lspec alias
var_defined('_lspec') ? $_lspec = void
lspec->formatResults('d')
lspec->indentor(' ')
lspec->suppressOutput
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



#test_desc = "Testing a pending test: [lspec->numTests] & [lspec->numPending] increments while [lspec->numSuccesses] does not"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {}
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
    var(success) = false
    handle(error_code == error_code_lspecNotInTest &&
           error_msg  == error_msg_lspecNotInTest) => 
    {
        failure_clear
        error_reset
        $success = true
    }
    
    expect(true)
    if(not $success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

$success = false
describe("Something") => {
    it("does something") => {
        expect(true)
        $success = true
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Basic successful test increments [lspec->numSuccesses] & [lspec->numTestCases]"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        expect(true)
        expect(true)
    }
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTestCases != 2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Fails if one test case fails & increments [lspec->numFailures]"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        expect(true)
        expect(false)
    }
}
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Registers one test failing if multiple test cases fail"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        expect(false)
        expect(false)
    }
}
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[it] test fails when an error is raised"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        5/0
    }
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Tests for valid resultsFormat options"
lspec_setupForTesting
try => {
    var(success) = false
    handle(error_code == error_code_lspecInvalidResultFormat &&
           error_msg  == error_msg_lspecInvalidResultFormat) => 
    {
        failure_clear
        error_reset
        $success = true
    }
    
    lspec->formatResults('#')
    if(not $success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}
lspec->formatResults('p')
lspec->formatResults('d')



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
lspec->showOutput
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



#test_desc = "[lspec->runTime] is properly incremented for a test group"
lspec_setupForTesting
describe("Something") => {
    context("nested test group") => {
        it("does something") => {
            sleep(2000)
        }
    }
}
if(2 != (lspec->runTime / 1000000)) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('Expected 2 but got ' + (lspec->runTime / 1000000))
    abort
}



#test_desc = "[lspec->stop] causes an error if called in a test group"
lspec_setupForTesting
describe("Something") => {
    protect => {
        var(success) = false
        handle(error_code == error_code_lspecStopInTestGroup &&
               error_msg  == error_msg_lspecStopInTestGroup) => 
        { $success = true }
        lspec->stop
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->stop] causes an error if called in a test and causes that test to fail"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_lspecStopInTestGroup &&
                   error_msg  == error_msg_lspecStopInTestGroup) => 
            { $success = true }
            lspec->stop
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->stop] sets the [lspec->isStopped] flag"
lspec_setupForTesting
lspec->stop
if(not lspec->isStopped) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->stop] outputs final summary"
lspec_setupForTesting
lspec->stop
local(expected_result) = '\n\nFinished in ' + lspec->runTimeSec + ' seconds\n0 tests, 0 failures\n'
if(lspec->result != #expected_result) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->stop] outputs test and failure in singular fashion for one test which also fails"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        expect(false)
    }
}
lspec->stop
local(expected_result) = '\n\nFinished in ' + lspec->runTimeSec + ' seconds\n1 test, 1 failure\n'
if(not lspec->result->endsWith(#expected_result)) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "After [lspec->stop], a new [describe] causes an [lspec_clearSuite] (clears all but options)"
lspec_setupForTesting
describe("Something") => {
    it("is pending") => {}
    it("fails something") => { expect(false) }
    it("has a success") => { expect(true) }
}
lspec->stop
describe("Another thing") => {}
if(lspec->result != "Another thing") => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(not lspec->isOutputSuppressed) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->failures] is blank when no tests have failed"
lspec_setupForTesting
describe("Something") => {
    it("is pending") => {}
    it("has a success") => { expect(true) }
}
if(lspec->failures != '') => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->failures] is properly formatted for one failure"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        expect(false)
    }
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect(false)
' + lspec->indentor + '   ' + lspec->indentor + 'expected: true
' + lspec->indentor + '   ' + lspec->indentor + '     got: false
' + lspec->indentor + '   # 521:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->failures] is properly formatted for multiple failure"
lspec_setupForTesting
describe("Something") => {
    it("does something 1") => {
        expect(false)
    }
    
    it("does something 2") => {
        expect(false)
    }
    
    it("does something 3") => {
        expect(false)
    }
}
local(expected) = '\n\nFailures:'
local(start_line) = 542
loop(3) => {
    #expected += '\n
' + lspec->indentor + loop_count + ') Something does something ' + loop_count + '
' + lspec->indentor + '   Failure/Error: expect(false)
' + lspec->indentor + '   ' + lspec->indentor + 'expected: true
' + lspec->indentor + '   ' + lspec->indentor + '     got: false
' + lspec->indentor + '   # ' + (#start_line + ((loop_count - 1)*4)) + ':9 ' + currentCapture->callsite_file
}
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->failures] is properly formatted for a failure due to an error"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        5/0
        expect(true)
    }
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: Unexpected Error!
' + lspec->indentor + '   ' + lspec->indentor + 'Error Code: ' + error_code_divideByZero + '
' + lspec->indentor + '   ' + lspec->indentor + ' Error Msg: ' + error_msg_divideByZero + '
' + lspec->indentor + '   # 574:5 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}


#test_desc = "[lspec->failures] is properly formatted for a failure in a nested describe"
lspec_setupForTesting
describe("Something") => {
    describe("calls something") => {
        it("does something") => {
            expect(false)
        }
    }
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something calls something does something
' + lspec->indentor + '   Failure/Error: expect(false)
' + lspec->indentor + '   ' + lspec->indentor + 'expected: true
' + lspec->indentor + '   ' + lspec->indentor + '     got: false
' + lspec->indentor + '   # 597:13 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->pendings] is blank when no tests are pending"
lspec_setupForTesting
describe("Something") => {
    it("has a success") => { expect(true) }
    it("has a failure") => { expect(false) }
}
if(lspec->pendings != '') => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->pendings] is properly formatted for one pending"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {}
}
local(expected) = '\n\nPending:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   ' + lspec->indentor + '# 632:5 ' + currentCapture->callsite_file
if(lspec->pendings != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[lspec->pendings] is properly formatted for multiple pending"
lspec_setupForTesting
describe("Something") => {
    it("does something 1") => {}
    
    it("does something 2") => {}
    
    it("does something 3") => {}
}
local(expected) = '\n\nPending:'
local(start_line) = 648
loop(3) => {
    #expected += '\n
' + lspec->indentor + loop_count + ') Something does something ' + loop_count + '
' + lspec->indentor + '   ' + lspec->indentor + '# ' + (#start_line + ((loop_count - 1)*2)) + ':5 ' + currentCapture->callsite_file
}
if(lspec->pendings != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}