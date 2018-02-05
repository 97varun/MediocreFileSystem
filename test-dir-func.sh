#!/bin/bash
cd mount-point
mkdir a
cd a
mkdir aa
cd aa
mkdir aaa
cd ..
cd ..
mkdir b
cd b
mkdir bb
mkdir bbb
cd ..
# you should have tree installed!
tree
cd ..
echo '
expected output:

	.
	├── a
	│   └── aa
	│       └── aaa
	└── b
	    ├── bb
	    └── bbb


'
