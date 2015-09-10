LSpec
=====

LSpec is a [Test-Driven Development](http://en.wikipedia.org/wiki/Test-driven_development) tool for the Lasso language that gets its inspiration from [RSpec](https://github.com/rspec/rspec).


Terminology
-----------

<dl>
    <dt>Expectation</dt><dd>Used to define expected outcomes within a test. (Each [expect] statement)</dd>
    <dt>Test</dt><dd>A grouping of expectations and related code to test specific functionality. (Each [it] block.)</dd>
    <dt>Test Group</dt><dd>A grouping of related tests. (Each [describe] block.)</dd>
    <dt>Test Suite</dt><dd>Compilation of all the test groups.</dd>
</dl>


Installation
------------

### Pre-compiled Libraries

1. Click the "Downloads" menu option at the top of this page.
2. Choose the proper download for your platform.
3. Decompress the files and move lspec to `$LASSO9_HOME/bin/` and lspec.so or lspec.dylib into `$LASSO9_HOME/LassoLibraries/`.

### Compile From Source

    $> cd where/you/want/the/source/installed/
    $> git clone https://bitbucket.org/bfad/lspec.git
    $> cd lspec
    $> make
    $> make install

_Note: If you're compiling on Mac OS X, you'll need the Xcode command-line tools installed._

### Set Your Path

The installation instructions above specified that the lspec command utility be installed into `$LASSO9_HOME/bin/`. You may want to add that to your shell's `PATH` so you can just type `lspec` and not `$LASSO9_HOME/bin/lspec` from the command-line. (Check your shell's documentation to determine how to setup your PATH environment variable.)


Example
-------

First, create a file named zoo.spec.inc and write a test describing functionality you wish you had. (It's important that the test file ends with the extension '.spec.inc'.)

    describe('Zoo') => {
        describe('addAnimal') => {
            it('inserts an animal into the zoo and increases numberOfAnimals by 1') => {
                local(menagerie) = zoo
                local(num_animals_start) = #menagerie->numberOfAnimals
                local(num_animals_end)
                
                #menagerie->addAnimal(animal('Rhino'))
                #num_animals_end = #menagerie->numberOfAnimals
                
                expect(#menagerie->hasA('Rhino'))
                expect(#num_animals_end == 1 + #num_animals_start)
            }
        }
    }
    
Then run the test and watch it fail.

    $> lspec zoo.spec.inc
    F

    Failures:

        1) Zoo addAnimal inserts an animal into the zoo and increases numberOfAnimals by 1
           Failure/Error: Unexpected Error!
               Error Code: -9948
                Error Msg: Definition Not Found: zoo()
           # 3:9 /Path/To/zoo.spec.inc

    Finished in 0.206136 seconds
    1 test, 1 failure

Now add the following code to the beginning of zoo.spec.inc. (Usually you would keep the tests in separate files and have them first include the code you are testing.)
    
    define zoo => type {
        data private animals = array

        public hasA(name::string) => {
            with animal in .animals do {
                #name == #animal->name?
                    return true
            }
            return false
        }

        public addAnimal(creature::animal) => {
            .'animals'->insert(#creature)
        }

        public numberOfAnimals => .animals->size
    }

    define animal => type {
        data private name::string
        public name => .'name'

        public onCreate(name::string) => {
            .'name' = #name
        }
    }

With this code in place, re-run the test and watch it pass.

    $> lspec zoo.spec.inc
    .

    Finished in 0.147116 seconds
    1 test, 0 failures


License
-------

Copyright 2011 Bradley Lindsay

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

>    [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
