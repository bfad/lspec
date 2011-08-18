local(test_desc) = "Given a describe with 2 [it] blocks, it should return 2 as the number of tests"
lspec_setupForTesting
describe("Something") => {
    it("Does something") => {}
    it("Does something else") => {}
}
if(lspec->suite->first->numTests != 2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[describe] sub-test groups properly handled at the end of tests & outputed correctly"
lspec_setupForTesting
local(flag1, flag2) = (:false, false)
describe("Something") => {
    it("Does something") => {}
    describe("As an aspect") => {
        #flag1 = true
        describe("Sub-aspect") => {
            #flag2 = true
        }
        it("Does something cool") => {
            #flag2 = false
        }
    }
    it("Does something else") => {
        #flag1 = false
        #flag2 = false
    }
}
if(not #flag1 or not #flag2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "Given a [context] with 2 [it] blocks, it should return 2 as the number of tests"
lspec_setupForTesting
context("Something") => {
    it("Does something") => {}
    it("Does something else") => {}
}
if(lspec->suite->first->numTests != 2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "[context] sub-test groups properly handled at the end of tests & outputed correctly"
lspec_setupForTesting
local(flag1, flag2) = (:false, false)
context("Something") => {
    it("Does something") => {}
    context("As an aspect") => {
        #flag1 = true
        context("Sub-aspect") => {
            #flag2 = true
        }
        it("Does something cool") => {
            #flag2 = false
        }
    }
    it("Does something else") => {
        #flag1 = false
        #flag2 = false
    }
}
if(not #flag1 or not #flag2) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if an [beforeEach] method isn't inside a [describe] method"
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
    
    beforeEach => {}
    if(not #success) => {
        stdoutnl('Test Failed\n' + #test_desc)
        stdoutnl(currentCapture->callStack->split('\n')->first)
        abort
    }
}

#success = false
describe("Something") => {
    beforeEach => {}
    #success = true
}
if(not #success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "It should throw an error if try to add a second [beforeEach] method"
lspec_setupForTesting
local(success) = false
describe("Something") => {
    beforeEach => {}
    try => {
        handle(error_code == error_code_lspecTestGroupHasBeforeEach &&
               error_msg  == error_msg_lspecTestGroupHasBeforeEach) => 
        {
            failure_clear
            error_reset
            #success = true
        }
        beforeEach => {}
    }
}
if(not #success) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}



#test_desc = "The [beforeEach] code should be called before each test in a test group"
lspec_setupForTesting
local(num_calls) = 0
describe("Something") => {
    beforeEach => {
        #num_calls++
    }
    it("does one thing") => {}
    it("does two thing") => {}
    it("does six thing") => {}
}
if(3 != #num_calls) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}