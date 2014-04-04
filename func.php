<?php

if (function_exists($argv[1]))
	print_r(substr(call_user_func_array($argv[1], $argv), 8));
