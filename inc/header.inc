<?php
session_start();
if(!isset($_SESSION['personel_location']))
  $_SESSION['personel_location'] = '';


if($_SESSION['timezone']<>"")
   $tz = $_SESSION['timezone'];
else
   $tz = 'Europe/Warsaw';

$timestamp = time();
$dt = new DateTime("now", new DateTimeZone($tz)); //first argument "must" be a string
$dt->setTimestamp($timestamp); //adjust the object to correct timestamp
date_default_timezone_set($tz);



$lid = '';
$lid = preg_replace("/[^a-a0-9]+/", "", $_GET['lid']);



if($lid=="a")
   {
   $_SESSION['personel_location'] = '';
   }
elseif($lid>0)
   {
   $_SESSION['personel_location'] = $lid;
   }

$personel_location_selected = $_SESSION['personel_location'];


//if($_SESSION['location_id_limit']<>0)
//     $personel_location_selected = $_SESSION['location_id_limit'];


if($action<>'xlse')
  {

?>
<!DOCTYPE html>
<html lang="en">
<head>
 <title>

<?php echo $header_title; ?>

</title>
 <meta charset="utf-8">

<!--<link rel="stylesheet" href="https://yui-s.yahooapis.com/pure/0.6.0/pure-min.css">-->

<link rel="stylesheet" href="https://unpkg.com/purecss@1.0.0/build/pure-min.css" integrity="sha384-nn4HPE8lTHyVtfCBi5yW9d20FjT8BJwUXyWZT9InLYax14RDjBj46LmSztkmNP9w" crossorigin="anonymous">

<!--<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">-->
        <link rel="shortcut icon"  href="inc/favicon.png"/>
        <link rel="icon"  href="inc/favicon-114x114.png" sizes="114x114"/>

<meta name="theme-color" content="#333000">


<link rel="manifest" href="mob_manifest.json">



<link rel="stylesheet" href="inc/addon2.css">

<meta name="viewport" content="width=device-width, initial-scale=1">



</head>
<body>


<?php
   };


if($_SESSION['limit_site']<>"" and $_SERVER['SCRIPT_NAME']<>'/'.$_SESSION['limit_site'])
  {
  header('location: '.$_SESSION['limit_site']);
  exit;
  }
?>
