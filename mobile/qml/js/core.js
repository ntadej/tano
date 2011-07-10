function addPage(file) {
    var component = Qt.createComponent(file)

    if (component.status == Component.Ready)
        pageStack.push(component);
    else
        console.log("Error loading component:", component.errorString());
}

function replacePage(file) {
    var component = Qt.createComponent(file)

    if (component.status == Component.Ready)
        pageStack.replace(component);
    else
        console.log("Error loading component:", component.errorString());
}
