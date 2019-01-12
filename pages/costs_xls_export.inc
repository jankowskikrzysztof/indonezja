<?php

  /** Error reporting */
//error_reporting(E_ALL);
//ini_set('display_errors', TRUE);
//ini_set('display_startup_errors', TRUE);
//date_default_timezone_set('Europe/London');

if (PHP_SAPI == 'cli')
	die('This example should only be run from a Web Browser');

/** Include PHPExcel */
require_once dirname(__FILE__) . '/../inc/PHPExcel.php';


// Create new PHPExcel object
$objPHPExcel = new PHPExcel();

// Set document properties
$objPHPExcel->getProperties()->setCreator("Gruvi")
							 ->setLastModifiedBy("GRUVI")
							 ->setTitle("Gruvi Costs Document")
							 ->setSubject("Gruvi Costs Document")
							 ->setDescription("Gruvi Costs Document")
							 ->setKeywords("office 2007 gruvi costs")
							 ->setCategory("costs file");

   $objPHPExcel->setActiveSheetIndex(0)
            ->setCellValue('A1', 'Date')
            ->setCellValue('B1', 'Location')
            ->setCellValue('C1', 'Personel')
            ->setCellValue('D1', 'Group')
            ->setCellValue('E1', 'Description')
            ->setCellValue('F1', 'Trip')
            ->setCellValue('G1', 'Still to pay')
            ->setCellValue('H1', 'Amount Paid');

   $objPHPExcel->getActiveSheet()->getColumnDimension('A')->setWidth(12);
   $objPHPExcel->getActiveSheet()->getColumnDimension('B')->setWidth(25);
   $objPHPExcel->getActiveSheet()->getColumnDimension('C')->setWidth(10);
   $objPHPExcel->getActiveSheet()->getColumnDimension('D')->setWidth(20);
   $objPHPExcel->getActiveSheet()->getColumnDimension('E')->setWidth(60);
   $objPHPExcel->getActiveSheet()->getColumnDimension('F')->setWidth(30);
   $objPHPExcel->getActiveSheet()->getColumnDimension('G')->setWidth(12);
   $objPHPExcel->getActiveSheet()->getColumnDimension('H')->setWidth(12);

$i=0;
$i++;

if($personel_location_selected <> '')
    $sel_loc_pers = 'and location.id_location IN ('.$personel_location_selected.')';
else
    $sel_loc_pers = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
                              cash_book.id_cash_book, cash_book.date,cash_book.costs_bill_value, DAY(cash_book.date) as dzien,

                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name
                              FROM (personel, location, cash_book_group,cash_book)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              AND MONTH(`cash_book`.`date`) = '".$period_month."'
                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
                              ".$sel_loc_pers."
                              ".$location_limit."
                              and cash_book.type=2
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id
                              ORDER BY cash_book.date desc");

$stmt -> execute();


$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {

   $still_to_pay = 0; $still_to_pay = $row['costs_bill_value']-$row['value'];
   $trip = ''; if($row['txt_route']<>'') $trip = $row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'];

   $i++;
   $objPHPExcel->setActiveSheetIndex(0)
            ->setCellValue('A'.$i, $row['date'])
            ->setCellValue('B'.$i, $row['loc_name'])
            ->setCellValue('C'.$i, $row['pers_name'])
            ->setCellValue('D'.$i, $row['cb_name'])
            ->setCellValue('E'.$i, $row['desc'])
            ->setCellValue('F'.$i, $trip)
            ->setCellValue('G'.$i, $still_to_pay)
            ->setCellValue('H'.$i, $row['value']);

   if($personel_location_selected>0)
      $loc_filename = '_['.$row['loc_name'].']';

   }


// Rename worksheet
$objPHPExcel->getActiveSheet()->setTitle('gruvi-costs');


// Set active sheet index to the first sheet, so Excel opens this as the first sheet
$objPHPExcel->setActiveSheetIndex(0);


// Redirect output to a client’s web browser (Excel2007)
header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
header('Content-Disposition: attachment;filename="costs-gruvi_'.$period.$loc_filename.'.xlsx"');
header('Cache-Control: max-age=0');
// If you're serving to IE 9, then the following may be needed
header('Cache-Control: max-age=1');

// If you're serving to IE over SSL, then the following may be needed
header ('Expires: Mon, 26 Jul 1997 05:00:00 GMT'); // Date in the past
header ('Last-Modified: '.gmdate('D, d M Y H:i:s').' GMT'); // always modified
header ('Cache-Control: cache, must-revalidate'); // HTTP/1.1
header ('Pragma: public'); // HTTP/1.0

$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
$objWriter->save('php://output');
exit;

?>