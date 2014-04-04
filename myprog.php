#!/usr/bin/php
<?php
session_start();

echo json_encode($_SESSION);
