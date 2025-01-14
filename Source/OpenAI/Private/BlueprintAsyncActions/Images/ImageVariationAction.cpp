// OpenAI, Copyright LifeEXE. All Rights Reserved.

#include "BlueprintAsyncActions/Images/ImageVariationAction.h"
#include "Provider/OpenAIProvider.h"
#include "API/API.h"

UImageVariationAction* UImageVariationAction::CreateImageVariation(
    const FOpenAIImageVariation& ImageVariation, const FOpenAIAuth& Auth, const FString& URLOverride)
{
    auto* ImageVariationAction = NewObject<UImageVariationAction>();
    ImageVariationAction->ImageVariation = ImageVariation;
    ImageVariationAction->Auth = Auth;
    ImageVariationAction->URLOverride = URLOverride;
    return ImageVariationAction;
}

void UImageVariationAction::Activate()
{
    auto* Provider = NewObject<UOpenAIProvider>();
    Provider->OnCreateImageVariationCompleted().AddUObject(this, &ThisClass::OnCreateImageVariationCompleted);
    Provider->OnRequestError().AddUObject(this, &ThisClass::OnRequestError);
    TryToOverrideURL(Provider);
    Provider->CreateImageVariation(ImageVariation, Auth);
}

void UImageVariationAction::OnCreateImageVariationCompleted(const FImageVariationResponse& Response)
{
    OnCompleted.Broadcast(Response, {});
}

void UImageVariationAction::OnRequestError(const FString& URL, const FString& Content)
{
    OnCompleted.Broadcast({}, FOpenAIError{Content, true});
}

void UImageVariationAction::TryToOverrideURL(UOpenAIProvider* Provider)
{
    if (URLOverride.IsEmpty()) return;

    OpenAI::V1::FOpenAIEndpoints Endpoints;
    Endpoints.ImageVariations = URLOverride;
    const auto API = MakeShared<OpenAI::V1::GenericAPI>(Endpoints);
    Provider->SetAPI(API);
}
