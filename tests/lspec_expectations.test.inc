// Expect extensions
local(test_desc) = "[expect->error] raises an error if it's not passed a block"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        try => {
            local(success) = false
            handle(error_code == error_code_missingGivenBlock &&
                   error_msg  == error_msg_missingGivenBlock) => 
            { #success = true }
            expect->error(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup)
        }
    }
}
if(not #success) => {
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



/*
#test_desc = "[expect->error] fails when it gets an unexpected error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {}
    it("Does something else") => {}
}
if(lspec->suite->first->numTests != 2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}*/
/////////
//If it gets an unexpected error, should it raise it? (and be caught there?)
////////


#test_desc = "[expect->error] fails when it doesn't get the expected error"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {
        expect->error(error_code_lspecNotInTestGroup, error_msg_lspecNotInTestGroup) => {
            fail(error_code_lspecNotInTest, error_msg_lspecNotInTest)
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