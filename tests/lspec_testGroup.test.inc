local(test_desc) = "Given a describe with 2 it blocks, it should return 2 as the number of tests"
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



#test_desc = "Sub-test groups properly handled at the end of tests & outputed correctly"
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



#test_desc = "Given a context with 2 it blocks, it should return 2 as the number of tests"
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



#test_desc = "Sub-test groups properly handled at the end of tests & outputed correctly"
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