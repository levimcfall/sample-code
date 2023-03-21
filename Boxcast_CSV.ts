function generateBoxcastCSVCathedral() {
  let scheduleSheet = SpreadsheetApp.openById(<censored>).getSheetByName("Cathedral")
  let data = []
  
  let dateColumn = 1
  let typeColumn = 2
  let musicStartTimeColumn = 3
  let ministerColumn = 6
  let namesColumn = 12
  let csvStatusColumn = 17
  let familyStartTime = "9:10 AM"
  let adultStartTime = "10:35 AM"
  let familyStopTime = "10:20 AM"
  let adultStopTime =  "12:30 PM"
  let weddingDuration = 2
  let memorialDuration = 2
  let vespersDuration = 2
  let specialEventDuration = 2.5
  let startTimeOffset = -5/60 // in hours

  let title = ""
  let description = ""
  let startTime = ""
  let stopTime = ""
  let source = "Atem Mini Extreme ISO"
  let channel = "Bryn Athyn Cathedral"
  let visibility = "Private/Test"
  let resolution = 1080

  let row = 2
  while (row < 100) {
    if (scheduleSheet.getRange(row, csvStatusColumn).getValue() == false && scheduleSheet.getRange(row, typeColumn).getValue().indexOf("*") == -1 && scheduleSheet.getRange(row, musicStartTimeColumn).getValue() != "") {
     let date = scheduleSheet.getRange(row, dateColumn).getValue()
     let type = scheduleSheet.getRange(row, typeColumn).getValue()
     let musicStartTime = getTimeFromDate(scheduleSheet.getRange(row, musicStartTimeColumn).getValue()) // custom function from different file
     let minister = scheduleSheet.getRange(row, ministerColumn).getValue()
     let names = scheduleSheet.getRange(row, namesColumn).getValue()
     let formattedDate = formatDateLong(date) // custum function from different file
     switch (type) {
      case "Family Service":
        title = `Family Service ${formattedDate}`
        startTime = familyStartTime
        stopTime = familyStopTime
        description = ""
        break;
      case "Adult Service":
        title = `Adult Service ${formattedDate}`
        startTime = adultStartTime
        stopTime = adultStopTime
        description = ""
        break;
      case "Memorial Service":
        title = `Memorial Service for ${names}`
        description = `Officiated by Rev. ${minister}`
        startTime = addHoursTo(musicStartTime, startTimeOffset) // custom function from different file.
        stopTime = addHoursTo(musicStartTime, memorialDuration)
        break;
      case "Wedding":
        title = `Wedding Ceremony for ${names}`
        description = `Officiated by Rev. ${minister}`
        startTime = addHoursTo(musicStartTime, startTimeOffset)
        stopTime = addHoursTo(musicStartTime, weddingDuration)
        break;
      case "Evening Vespers":
        title = `Evening Vespers ${formattedDate} (${names})`
        description = ""
        startTime = addHoursTo(musicStartTime, startTimeOffset)
        stopTime = addHoursTo(musicStartTime, vespersDuration)
        break;
      case "Special Event":
        title = names
        startTime = addHoursTo(musicStartTime, startTimeOffset)
        stopTime = addHoursTo(musicStartTime, specialEventDuration)
        description = ""
        break;
      default:
        // Do nothing
     }
      let service = [title, description, source, formatDateShort(date), startTime, stopTime, visibility, channel, resolution] // If I were writing this now, I would use a class for this. 
      if (title != "") { data.push(service) }
      scheduleSheet.getRange(row, csvStatusColumn).setValue(true)
    }
    row += 1
  }
  if (data.length > 0) {
  let csvSheet = SpreadsheetApp.openById(<censored>).getSheetByName("CSV")
  csvSheet.getRange("A2:I150").clear()
  let r = 2
  let c = 1
  for (let service of data) {
    for (let item of service) {
      csvSheet.getRange(r,c).setValue(item)
      c += 1
    }
    c = 1
    r += 1
  }
  }

}