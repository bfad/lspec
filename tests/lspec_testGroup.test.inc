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
describe("Something") => {
    it("Does something") => {}
    describe("As an aspect") => {
        describe("Sub-aspect") => {}
        it("Does something cool") => {}
    }
    it("Does something else") => {}
}
local(my_result) = 'Something
' + lspec->indentor + 'Does something
' + lspec->indentor + 'Does something else
' + lspec->indentor + 'As an aspect
' + lspec->indentor + lspec->indentor + 'Does something cool
' + lspec->indentor + lspec->indentor + 'Sub-aspect'
if(lspec->result != #my_result) => {
    stdoutnl('Test Failed\n' + #test_desc)
    stdoutnl(currentCapture->callStack->split('\n')->first)
    abort
}