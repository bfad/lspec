// Expect extensions
local(test_desc) = "[expect->error] raises an error if it's not passed a block"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_missingGivenBlock &&
                   error_msg  == error_msg_missingGivenBlock) => 
            { $success = true }
            expect->error(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->error] passes when it gets the expected error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->error(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup) => {
            fail(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        }
    }
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->error] fails when it gets an unexpected error & doesn't re-raise it"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        try => {
            var(success) = true
            handle(error_code == error_code_divideByZero &&
                   error_msg  == error_msg_divideByZero) => 
            { $success = false }
            expect->error(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup) => {
                5/0
            }
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->error(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup) => {
' + lspec->indentor + '   ' + lspec->indentor + 'expected: ' + error_code_lspecNotInTestGroup + ': ' + error_msg_lspecNotInTestGroup + '
' + lspec->indentor + '   ' + lspec->indentor + '     got: ' + error_code_divideByZero + ': ' + error_msg_divideByZero + '
' + lspec->indentor + '   # 84:13 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->error] fails when it doesn't get any error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->error(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup) => {}
    }
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->error(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup) => {}
' + lspec->indentor + '   ' + lspec->indentor + 'expected: ' + error_code_lspecNotInTestGroup + ': ' + error_msg_lspecNotInTestGroup + '
' + lspec->indentor + '   ' + lspec->indentor + '     got: ' + error_code_noerror + ': ' + error_msg_noerror + '
' + lspec->indentor + '   # 133:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



local(test_desc) = "[expect->error] with no args raises an error if it's not passed a block"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_missingGivenBlock &&
                   error_msg  == error_msg_missingGivenBlock) => 
            { $success = true }
            expect->error
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->error] with no args passes when it gets the expected error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->error => {
            fail(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        }
    }
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->error] with no args fails when it doesn't get any error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->error => {}
    }
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->error => {}
' + lspec->indentor + '   ' + lspec->indentor + 'expected: Any Error
' + lspec->indentor + '   ' + lspec->indentor + '     got: ' + error_code_noerror + ': ' + error_msg_noerror + '
' + lspec->indentor + '   # 247:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



//
// [expect->errorCode] //
//
local(test_desc) = "[expect->errorCode] raises an error if it's not passed a block"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_missingGivenBlock &&
                   error_msg  == error_msg_missingGivenBlock) => 
            { $success = true }
            expect->errorCode(error_code_lspecNotInTestGroup)
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->errorCode] passes when it gets the expected error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->errorCode(error_code_lspecNotInTestGroup) => {
            fail(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        }
    }
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->errorCode] fails when it gets an unexpected error & doesn't re-raise it"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        try => {
            var(success) = true
            handle(error_code == error_code_divideByZero &&
                   error_msg  == error_msg_divideByZero) => 
            { $success = false }
            expect->errorCode(error_code_lspecNotInTestGroup) => {
                5/0
            }
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->errorCode(error_code_lspecNotInTestGroup) => {
' + lspec->indentor + '   ' + lspec->indentor + 'expected: ' + error_code_lspecNotInTestGroup + '
' + lspec->indentor + '   ' + lspec->indentor + '     got: ' + error_code_divideByZero + '
' + lspec->indentor + '   # 369:13 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->errorCode] fails when it doesn't get any error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->errorCode(error_code_lspecNotInTestGroup) => {}
    }
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->errorCode(error_code_lspecNotInTestGroup) => {}
' + lspec->indentor + '   ' + lspec->indentor + 'expected: ' + error_code_lspecNotInTestGroup + '
' + lspec->indentor + '   ' + lspec->indentor + '     got: ' + error_code_noerror + '
' + lspec->indentor + '   # 418:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



local(test_desc) = "[expect->errorCode] with no args raises an error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_methodNotFound) => { $success = true }
            expect->errorCode => {}
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



//
// [expect->errorMsg] //
//
local(test_desc) = "[expect->errorMsg] raises an error if it's not passed a block"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_missingGivenBlock &&
                   error_msg  == error_msg_missingGivenBlock) => 
            { $success = true }
            expect->errorMsg(error_msg_lspecNotInTestGroup)
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->errorMsg] passes when it gets the expected error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->errorMsg(error_msg_lspecNotInTestGroup) => {
            fail(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        }
    }
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->errorMsg] fails when it gets an unexpected error & doesn't re-raise it"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        try => {
            var(success) = true
            handle(error_code == error_code_divideByZero &&
                   error_msg  == error_msg_divideByZero) => 
            { $success = false }
            expect->errorMsg(error_msg_lspecNotInTestGroup) => {
                5/0
            }
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->errorMsg(error_msg_lspecNotInTestGroup) => {
' + lspec->indentor + '   ' + lspec->indentor + 'expected: ' + error_msg_lspecNotInTestGroup + '
' + lspec->indentor + '   ' + lspec->indentor + '     got: ' + error_msg_divideByZero + '
' + lspec->indentor + '   # 579:13 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->errorMsg] fails when it doesn't get any error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->errorMsg(error_msg_lspecNotInTestGroup) => {}
    }
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->errorMsg(error_msg_lspecNotInTestGroup) => {}
' + lspec->indentor + '   ' + lspec->indentor + 'expected: ' + error_msg_lspecNotInTestGroup + '
' + lspec->indentor + '   ' + lspec->indentor + '     got: ' + error_msg_noerror + '
' + lspec->indentor + '   # 628:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



local(test_desc) = "[expect->errorMsg] with no args raises an error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_methodNotFound) => { $success = true }
            expect->errorMsg => {}
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



// [expect->stdout] //
#test_desc = "[expect->stdout] raises an error if it's not passed a block"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_missingGivenBlock &&
                   error_msg  == error_msg_missingGivenBlock) => 
            { $success = true }
            expect->stdout('Lager, sir, is regal.')
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->stdout] passes when it gets the expected data sent to stdout"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        // Need to make sure it's not sending anything to stdout
        // save stdout using ->dup
        // create a local conduit
        // close real stout, reopen it on our write pipe using dup2 system call
        // make a file object here which we read stout from
        local(saveStdout = file_stdout->fd->dup)
        local(readFd, writeFd) = io_file_pipe
        #writeFd->dup2(file_stdout->fd)
        local(readTest = file(#readFd))
        
        handle => {
            // set stout back using our save
            #saveStdout->dup2(file_stdout->fd)
        }
        
        expect->stdout('Lager, sir, is regal.') => {
            stdout('Lager, sir, is regal.')
        }
        
        // read test data
        var(didRead = #readTest->readString)
    }
}
if($didRead != '') => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->stdout] fails when it gets an unexpected error & re-raises it"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            var(success) = false
            handle(error_code == error_code_divideByZero &&
                   error_msg  == error_msg_divideByZero) => 
            { $success = true }
            expect->stdout('Lager, sir, is regal.') => {
                5/0
            }
        }
    }
}
if(not $success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something Does something
' + lspec->indentor + '   Failure/Error: Unexpected Error!
' + lspec->indentor + '   ' + lspec->indentor + 'Error Code: ' + error_code_divideByZero + '
' + lspec->indentor + '   ' + lspec->indentor + ' Error Msg: ' + error_msg_divideByZero + '
' + lspec->indentor + '   # 804:5 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->stdout] fails when stdout doesn't get the expected data"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->stdout('Lager, sir, is regal.') => {}
    }
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->stdout(\'Lager, sir, is regal.\') => {}
' + lspec->indentor + '   ' + lspec->indentor + 'expected: Lager, sir, is regal.
' + lspec->indentor + '   ' + lspec->indentor + '     got: 
' + lspec->indentor + '   # 859:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->null] succeeds when it is passed a null value"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->null(null) => {}
    }
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->null] fails when it is not passed a null value"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->null('Lager, sir, is regal.') => {}
    }
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->null(\'Lager, sir, is regal.\') => {}
' + lspec->indentor + '   ' + lspec->indentor + 'expected: null
' + lspec->indentor + '   ' + lspec->indentor + '     got: "Lager, sir, is regal."
' + lspec->indentor + '   # 930:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect(expect, got)] succeeds when expect == got"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect('Fast', 'Fast')
    }
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect(expect, got)] fails when expect != got"
lspec_setupForTesting
describe("Something") => {
    it("does something") => {
        expect('Fast', 'Slow')
    }
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect(\'Fast\', \'Slow\')
' + lspec->indentor + '   ' + lspec->indentor + 'expected: Fast
' + lspec->indentor + '   ' + lspec->indentor + '     got: Slow
' + lspec->indentor + '   # 1001:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    stdoutnl('\nExpected\n' + #expected)
    stdoutnl('\nReceived\n' + lspec->failures)
    abort
}



#test_desc = "[expect->valueIsA] succeeds when it is passed a value that matches the passed tag using ->isA"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->valueIsA(array, ::trait_forEach) => {}
    }
}
if(lspec->numTests != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numPending != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numSuccesses != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
if(lspec->numFailures != 0) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[expect->valueIsA] fails when it is not passed a value that doesn't match the passed tag using ->isA"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->valueIsA(0, ::trait_forEach) => {}
    }
}
if(lspec->numTests != 1) => {
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
if(lspec->numFailures != 1) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}
local(expected) = '\n\nFailures:\n
' + lspec->indentor + '1) Something does something
' + lspec->indentor + '   Failure/Error: expect->valueIsA(0, ::trait_forEach) => {}
' + lspec->indentor + '   ' + lspec->indentor + 'expected: trait_forEach
' + lspec->indentor + '   ' + lspec->indentor + '     got: 
' + lspec->indentor + '   # 1074:9 ' + currentCapture->callsite_file
if(lspec->failures != #expected) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}