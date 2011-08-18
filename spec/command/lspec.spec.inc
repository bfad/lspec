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
        local(test_file) = file('/tmp/lspec.test')
        
        it('executes the file') => {
            #test_file->doWithClose => {
                #test_file->openTruncate
                #test_file->writeString("file('" + #test_file->path + "')->openTruncate&close")
            }
            
            local(stdout, stderr) = call_lspec((:#test_file->path))
            
            expect->null(#stderr)
            expect('', #test_file->readString)
        }
        
        it('displays the "no test" summary') => {
            local(expected) = '\n\nFinished in ' + lspec->runTimeSec + ' seconds\n0 tests, 0 failures\n'
            
            #test_file->doWithClose => {
                #test_file->openTruncate
            }
            
            local(stdout, stderr) = call_lspec((:#test_file->path))
            
            expect->null(#stderr)
            expect(#expected, #stdout)
        }
    }
}

lspec->stop