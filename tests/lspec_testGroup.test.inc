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
