var(lspec_path) = 'command/lspec'

describe('rspec command') => {
    context('given a file path with no test groups') => {
        local(my_process) = sys_process
        local(test_file)  = file('/tmp/lspec.test')
        #test_file->doWithClose => {
            #test_file->openTruncate
            #test_file->writeString("file('" + #test_file->path + "')->openTruncate&close")
        }
        
        it('executes the file and displays the "no test" summary') => {
            local(stdout, stderr)
            local(expected) = '\n\nFinished in ' + lspec->runTimeSec + ' seconds\n0 tests, 0 failures\n'
            
            #my_process->open($lspec_path, (:#test_file->path))
            #my_process->wait
            #stdout = #my_process->read
            #stderr = #my_process->readError
            
            expect('' == #test_file->readString)
            expect->null(#stderr)
            expect(#expected == #stdout)
        }
    }
}

lspec->stop