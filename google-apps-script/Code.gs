/**
 * Google Apps Script for logging ESP8266 DHT11 readings to a sheet.
 *
 * 1. Create a Google Sheet
 * 2. Extensions -> Apps Script -> paste this file
 * 3. Set SPREADSHEET_ID below
 * 4. Deploy -> New deployment -> Web app (execute as me, anyone can access)
 * 5. Point PushingBox CustomURL at the deployed web app URL
 */

const SPREADSHEET_ID = 'YOUR_SPREADSHEET_ID';

function doGet(e) {
  if (!e || !e.parameter) {
    return ContentService.createTextOutput('No parameters');
  }

  const sheet = SpreadsheetApp.openById(SPREADSHEET_ID).getActiveSheet();
  const row = [
    new Date(),
    e.parameter.humidityData || '',
    e.parameter.celData || '',
    e.parameter.fehrData || '',
    e.parameter.hicData || '',
    e.parameter.hifData || '',
  ];

  sheet.appendRow(row);
  return ContentService.createTextOutput('Ok');
}
