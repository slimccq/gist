#!/usr/bin/python
# -*-coding:utf-8-*-

import os
import subprocess


svn_template = """// Auto-generated by `go:generate`, DO NOT EDIT!
package main

// `go:generate` produced code, DO NOT EDIT!
import (
    "fmt"
    "runtime"
)

func GetVersion() string {
    return fmt.Sprintf("rev %s, %s (built w/%%s)", runtime.Version())
}
"""

git_template = """// Auto-generated by `go:generate`, DO NOT EDIT!
package main

import (
    "fmt"
    "runtime"
)

func GetVersion() string {
    return fmt.Sprintf("rev %s %s (built w/%%s)", runtime.Version())
}
"""

# current SVN version information
def gen_svn_infomation():
    cmd = "svn info $GOPATH"
    output = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()
    assert output[1] == None # stderr
    info = {}
    for line in output[0].split('\n'):
        items = line.split(': ')
        if len(items) == 2:
            info[items[0]] = items[1]
    # generate go source file
    revision = info['Revision']
    date = info['Last Changed Date']
    date = date[:date.index('(')]
    content = svn_template % (revision, date)
    open('version.go', 'w').write(content)



# last git commit info
def gen_git_information():
    cmd = 'git log --date=iso --abbrev-commit -1'
    output = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()
    assert output[1] == None # stderr
    lines = output[0].split('\n')
    assert len(lines) >= 3
    commit = ''
    date = ''
    for line in lines:
        if line.startswith('commit '):
            commit = line[7:].strip()
        if line.startswith('Date:'):
            date = line[6:].strip()

    assert len(commit) > 0
    assert len(date) > 0
    content = git_template % (commit, date)
    open('version.go', 'w').write(content)


# run command
def main():
    gopath = os.getenv('GOPATH')
    if not gopath.endswith('/'):
        gopath += '/'

    if os.path.exists(gopath + '.svn'): # is svn repo
        print('svn repo detected')
        gen_svn_infomation()
    elif os.path.exists(gopath + '.git'): # is git repo
        print('git repo detected')
        gen_git_information()


if __name__ == '__main__':
    main()