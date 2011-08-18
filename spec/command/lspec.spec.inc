var(lspec_path) = 'command/lspec'
define call_lspec(args::staticarray=(:)) => {
    local(stdout, stderr)
    local(my_process) = sys_process
    
    #my_process->open($lspec_path, #args)
    #my_process->wait
    #stdout = #my_process->read
    #stderr = #my_process->readError
    
    return (:#stdout, #stderr)
}

describe('rspec command') => {
    context('given a file path with no test groups') => {
        it('executes the file and displays the "no test" summary') => {
            local(expected)   = '\n\nFinished in ' + lspec->runTimeSec + ' seconds\n0 tests, 0 failures\n'
            local(test_file)  = file('/tmp/lspec.test')
            
            #test_file->doWithClose => {
                #test_file->openTruncate
                #test_file->writeString("file('" + #test_file->path + "')->openTruncate&close")
            }
            
            local(stdout, stderr) = call_lspec((:#test_file->path))
            
            expect('', #test_file->readString)
            expect->null(#stderr)
            expect(#expected, #stdout)
        }
    }
}

lspec->stop