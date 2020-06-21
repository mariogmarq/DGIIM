#!/bin/bash
lastlog | grep `users` | awk '{ print FS $3 FS $4 FS $5 FS $6 }'

